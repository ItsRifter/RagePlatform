// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "InputActionValue.h"
#include "RageCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMapping, 0);
	}

	if (RestartWidgetBP)
	{
		RestartWidget = CreateWidget(GetWorld(), RestartWidgetBP);
		RestartWidget->AddToViewport();
		RestartWidgetRef = Cast<URRestartWidget>(RestartWidget);
	}

	SavedMaxAcceleration = GetCharacterMovement()->MaxAcceleration;

	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();
	if (PlayerController)
	{
		StartControllerRotation = PlayerController->GetControlRotation();
	}

	StartCameraRelativeLocation = Camera->GetRelativeLocation();

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
}

void ARageCharacter::OnDeathDelegate(const FText& DeathText)
{
	if (RestartWidgetRef)
	{
		RestartWidgetRef->DeathText->SetText(DeathText);
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

void ARageCharacter::Jump()
{
	ACharacter::Jump();
}

void ARageCharacter::PauseGame()
{
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
/// <summary>
/// TODO: Need to remove these.
/// </summary>
void ARageCharacter::Death()
{
	if (!bIsAlive)
	{
		return;
	}

	bIsAlive = false;

	OnDeath();
}

void ARageCharacter::RestartMenu() const
{
	if (PlayerController)
	{
		RestartWidget->SetVisibility(ESlateVisibility::Visible);
		PlayerController->SetShowMouseCursor(true);
		GetCharacterMovement()->MaxAcceleration = 0.f;
		GetCharacterMovement()->StopMovementImmediately();

		const FInputModeUIOnly InputModeDataUI;
		PlayerController->SetInputMode(InputModeDataUI);
	}
}

void ARageCharacter::CameraShake()
{
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
