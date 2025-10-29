// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "InputActionValue.h"
#include "RageCharacter.h"
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
	bRestarted = true;
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
	TimeCounter();

	/*if (GameInstance)
	{
		const FString TimeString = FString::Printf(TEXT("%f"),GameInstance->TimeVar);
		GEngine->AddOnScreenDebugMessage(-1,.5f,FColor::Red,TimeString); 
	}*/
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
	SetActorLocation(StartLocation);
	SetActorRotation(StartRotation);
	Camera->SetRelativeLocation(StartCameraRelativeLocation);
	PlayerController->SetControlRotation(StartControllerRotation);
}

void ARageCharacter::Move(const FInputActionValue& Value)
{
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
		PauseWidgetRef->SetVisibility(ESlateVisibility::Visible);
		PlayerController->SetShowMouseCursor(true);
		const FInputModeUIOnly InputModeUIOnly;
		PlayerController->SetInputMode(InputModeUIOnly);

		const int32 Index = UKismetMathLibrary::RandomIntegerInRange(0,PauseTexts.Num() - 1);
		PauseWidgetRef->PauseText->SetText(PauseTexts[Index]);
	}
	UGameplayStatics::SetGamePaused(this,true);
}

void ARageCharacter::Look(const FInputActionValue& Value)
{
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

void ARageCharacter::TimeCounter() const
{
	if (GameInstance)
	{
		if (GameInstance->bCanCountGameTime)
		{
			GameInstance->GameTimeVar += UGameplayStatics::GetWorldDeltaSeconds(this);

			const int32 GameTotalSeconds = FMath::FloorToInt(GameInstance->GameTimeVar);
			const int32 GameMinutes = (GameTotalSeconds % 3600) / 60;
			const int32 GameSeconds = GameTotalSeconds % 60;
			const int32 GameMilliseconds = FMath::RoundToInt((GameInstance->TimeVar - GameTotalSeconds) * 100.0f);

			const FString GameTimeString = FString::Printf(TEXT("%02d:%02d:%02d"), GameMinutes, GameSeconds, GameMilliseconds);

			if (PauseWidgetRef && RestartWidgetRef)
			{
				PauseWidgetRef->GameTimeText->SetText(FText::FromString(GameTimeString));
				RestartWidgetRef->GameTimeText->SetText(FText::FromString(GameTimeString));
			}
		}
		
		if (!GameInstance->bCanCountLevelTime)
		{
			return;
		}
	
		GameInstance->TimeVar += UGameplayStatics::GetWorldDeltaSeconds(this);

		const int32 TotalSeconds = FMath::FloorToInt(GameInstance->TimeVar);
		const int32 Minutes = (TotalSeconds % 3600) / 60;
		const int32 Seconds = TotalSeconds % 60;
		const int32 Milliseconds = FMath::RoundToInt((GameInstance->TimeVar - TotalSeconds) * 100.0f);

		const FString TimeString = FString::Printf(TEXT("%02d:%02d:%02d"), Minutes, Seconds, Milliseconds);

		if (PauseWidgetRef && RestartWidgetRef)
		{
			PauseWidgetRef->TimeText->SetText(FText::FromString(TimeString));
			RestartWidgetRef->TimeText->SetText(FText::FromString(TimeString));
		}
	}
}
