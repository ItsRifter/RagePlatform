// Fill out your copyright notice in the Description page of Project Settings.
#include "RageCharacter.h"

#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/RPauseWidget.h"
#include "UI/RRestartWidget.h"

// Sets default values
ARageCharacter::ARageCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("MainCamera");
	Camera->SetupAttachment(GetMesh());

	Camera->bUsePawnControlRotation = true;
	StartCameraRelativeLocation = FVector::ZeroVector;
	RestartButtonVisibility = 1.f;

	PauseTexts.Add(FText::FromString(TEXT("You Paused the Game I Think you can't make it.")));
}

// Called when the game starts or when spawned
void ARageCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnDeath.AddDynamic(this, &ARageCharacter::OnDeathDelegate);
		GameInstance->OnGameRestart.AddDynamic(this, &ARageCharacter::OnRestartDelegate);
		GameInstance->OnGameEnd.AddDynamic(this, &ARageCharacter::OnGameEndDelegate);
		GameInstance->bCanCountLevelTime = false;
		GameInstance->bCanCountGameTime = false;
		GameInstance->bCanLook = false;
	}

	PlayerController = Cast<APlayerController>(Controller);
	
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	if (RestartWidgetBP)
	{
		RestartWidget = CreateWidget(GetWorld(), RestartWidgetBP);
		RestartWidget->AddToViewport();
		RestartWidgetRef = Cast<URRestartWidget>(RestartWidget);
	}

	SavedMaxAcceleration = 3072.0;
	
	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();
	
	if (PlayerController)
	{
		StartControllerRotation = PlayerController->GetControlRotation();
	}

	StartCameraRelativeLocation = Camera->GetRelativeLocation();

	if (PauseWidgetBP)
	{
		UUserWidget* PauseWidget = CreateWidget(GetWorld(),PauseWidgetBP);
		PauseWidgetRef = Cast<URPauseWidget>(PauseWidget);
		PauseWidget->AddToViewport();
	}

	bIsAlive = true;
}

// Called to bind functionality to input
void ARageCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARageCharacter::Move);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARageCharacter::Jump);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARageCharacter::Look);
		Input->BindAction(PauseAction, ETriggerEvent::Triggered, this, &ARageCharacter::PauseGame);
	}
}

void ARageCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CameraShake();
	TimeCounter(DeltaSeconds);
}

void ARageCharacter::OnDeathDelegate(const FText& DeathText)
{
	if (RestartWidgetRef)
	{
		RestartWidgetRef->DeathText->SetText(DeathText);
		GameInstance->DeathCount++;
		RestartWidgetRef->DeathsCountText->SetText(FText::AsNumber(GameInstance->DeathCount));
		GetWorld()->GetTimerManager().SetTimer(RestartWidgetRef->VisibilityTimer,[this]
		{
			RestartWidgetRef->PlayAnimation(RestartWidgetRef->RestartAnim);
			RestartWidgetRef->SizeBoxButtons->SetVisibility(ESlateVisibility::Visible);
		},RestartButtonVisibility,false);
	}

	RestartMenu();
}

void ARageCharacter::OnRestartDelegate()
{
	bRestrictKeyboard = false;
	GetCharacterMovement()->MaxAcceleration = SavedMaxAcceleration;
	Respawn();
}

void ARageCharacter::OnGameEndDelegate()
{
	if (GameInstance)
	{
		FPlayerStats TempPlayerStats;
		TempPlayerStats.PlayerName = GameInstance->PlayerStats.PlayerName;
		TempPlayerStats.LevelTime = GameInstance->PlayerStats.LevelTime;
		TempPlayerStats.DeathCount = FString::FromInt(GameInstance->DeathCount);
		TempPlayerStats.GameTime = GameInstance->GameTimeVar;
		
		GameInstance->PlayerStats = TempPlayerStats;
		SaveGame(GameInstance->PlayerStats);

		const FPlayerStats EmptyPlayerStat;
		GameInstance->PlayerStats = EmptyPlayerStat;
		GameInstance->LevelNumber = 0;
	}
}

void ARageCharacter::Move(const FInputActionValue& Value)
{
	if (bRestrictKeyboard)
	{
		return;
	}

	const FVector2D MoveValue = Value.Get<FVector2D>();

	if (IsValid(Controller))
	{
		const FRotator Rot = Controller->GetControlRotation();
		const FRotator RotY(0, Rot.Yaw, 0);

		const FVector ForwardDir = FRotationMatrix(RotY).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(RotY).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDir, MoveValue.Y);
		AddMovementInput(RightDir, MoveValue.X);
	}
}

void ARageCharacter::JumpTrigger()
{
	ARageCharacter::Jump();
}

void ARageCharacter::PauseGame()
{
	if (PauseWidgetRef)
	{
		if (UGameplayStatics::IsGamePaused(this))
		{
			UGameplayStatics::SetGamePaused(this,false);
			const FInputModeGameOnly InputModeGameOnly;
			PlayerController->SetInputMode(InputModeGameOnly);
			PlayerController->SetShowMouseCursor(false);
			PauseWidgetRef->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			PauseWidgetRef->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			PlayerController->SetShowMouseCursor(true);
			const FInputModeGameAndUI InputModeGameAndUI;
			PlayerController->SetInputMode(InputModeGameAndUI);

			const int32 Index = UKismetMathLibrary::RandomIntegerInRange(0,PauseTexts.Num() - 1);
			PauseWidgetRef->PauseText->SetText(PauseTexts[Index]);
			UGameplayStatics::SetGamePaused(this,true);
		}
	}
}

void ARageCharacter::Look(const FInputActionValue& Value)
{
	if (!GameInstance->bCanLook)
	{
		return;
	}
	
	const FVector2D LookValue = Value.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(LookValue.X);
		AddControllerPitchInput(LookValue.Y);
	}
}

void ARageCharacter::RestartMenu() 
{
	if (PlayerController)
	{
		bIsAlive = false;
		RestartWidget->SetVisibility(ESlateVisibility::Visible);
		PlayerController->SetShowMouseCursor(true);

		GetCharacterMovement()->MaxAcceleration = 0.f;
		GetCharacterMovement()->StopMovementImmediately();

		const FInputModeUIOnly InputModeDataUI;
		PlayerController->SetInputMode(InputModeDataUI);
	}
}

void ARageCharacter::CameraShake() const
{
	if (!PlayerController)
	{
		return;
	}

	if (GetVelocity().Length() > 0 && CanJump())
	{
		if (WalkCameraShake)
		{
			PlayerController->ClientStartCameraShake(WalkCameraShake);
		}
	}
	else
	{
		if (IdleCameraShake)
		{
			PlayerController->ClientStartCameraShake(IdleCameraShake);
		}
	}
}

FText ARageCharacter::ConvertToTime(const float TimeFloat)
{
	const int32 TotalSeconds = FMath::FloorToInt(TimeFloat);
	const int32 Minutes = (TotalSeconds % 3600) / 60;
	const int32 Seconds = TotalSeconds % 60;
	const int32 Milliseconds = FMath::RoundToInt((TimeFloat - TotalSeconds) * 100.0f);

	const FString TimeString = FString::Printf(TEXT("%02d:%02d:%02d"), Minutes, Seconds, Milliseconds);

	return FText::FromString(TimeString);
}

void ARageCharacter::TimeCounter(float DeltaSeconds)
{
	if (GameInstance)
	{
		if (GameInstance->bCanCountGameTime)
		{
			GameInstance->GameTimeVar += DeltaSeconds;

			if (PauseWidgetRef && RestartWidgetRef)
			{
				PauseWidgetRef->GameTimeText->SetText(ConvertToTime(GameInstance->GameTimeVar));
				RestartWidgetRef->GameTimeText->SetText(ConvertToTime(GameInstance->GameTimeVar));
			}
		}
		
		if (!GameInstance->bCanCountLevelTime)
		{
			return;
		}
	
		GameInstance->TimeVar += DeltaSeconds;

		if (PauseWidgetRef && RestartWidgetRef)
		{
			PauseWidgetRef->TimeText->SetText(ConvertToTime(GameInstance->TimeVar));
			RestartWidgetRef->TimeText->SetText(ConvertToTime(GameInstance->TimeVar));
		}
	}
}
