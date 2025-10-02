// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "RageCharacter.h"

#include "Blueprint/UserWidget.h"

// Sets default values
ARageCharacter::ARageCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Camera = CreateDefaultSubobject<UCameraComponent>("MainCamera");
	Camera->SetupAttachment(GetMesh());

	Camera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void ARageCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnPlayerDeath.AddDynamic(this, &ARageCharacter::OnDeathDelegate);
	}

	const APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMapping, 0);
	}
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
	}
}

void ARageCharacter::OnDeathDelegate(bool bIsDead)
{
	if (bIsDead)
	{
		OnDeath();
	}
	else
	{
		OnRespawn();
	}
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

void ARageCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookValue = Value.Get<FVector2D>();

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
	if (!IsAlive)
	{
		return;
	}

	IsAlive = false;

	OnDeath();
}

void ARageCharacter::Respawn()
{
	if (IsAlive)
	{
		return;
	} 

	IsAlive = true;

	OnRespawn();
}

void ARageCharacter::RestartMenu()
{
	if (RestartWidgetBP)
	{
		RestartWidget = CreateWidget(GetWorld(), RestartWidgetBP);
		RestartWidget->AddToViewport();

		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this,0))
		{
			PlayerController->SetShowMouseCursor(true);
			PlayerController->SetPause(true);

			const FInputModeUIOnly InputModeDataUI;
			PlayerController->SetInputMode(InputModeDataUI);
		}
	}
}
