// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RageCharacter.generated.h"

class ARTempCamera;
class URRestartWidget;
class UInputAction;
class UCameraComponent;
class UInputMappingContext;
class URGameInstance;
struct FInputActionValue;

UENUM(BlueprintType)
enum class EKillerTrap : uint8
{
	Spikes				UMETA(DisplayName = "Spikes"),
	Axe					UMETA(DisplayName = "SwingingAxe"),
	Saw					UMETA(DisplayName = "Sawblade"),
	Chandelier			UMETA(DisplayName = "Chandelier"),
	LavaPit				UMETA(DisplayName = "LavaPit"),
	PoisonPit			UMETA(DisplayName = "PoisonPit"),
	Explosion			UMETA(DisplayName = "Explosions"),
};

UCLASS()
class ARageCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARageCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Player")
	UCameraComponent* Camera;

	UPROPERTY()
	URGameInstance* GameInstance;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> RestartWidgetBP;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> FadeWidgetBP;

	UPROPERTY(EditDefaultsOnly, Category = "Restart")
	float RestartButtonVisibility;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PauseWidgetBP;

	UPROPERTY(EditDefaultsOnly)
	TArray<FText> PauseTexts;

	UPROPERTY()
	UUserWidget* RestartWidget;

	UPROPERTY()
	URRestartWidget* RestartWidgetRef;

	UPROPERTY(BlueprintReadWrite)
	class URPauseWidget* PauseWidgetRef;

	UPROPERTY(BlueprintReadWrite)
	FVector StartLocation;

	UPROPERTY(BlueprintReadWrite)
	FRotator StartRotation;

	UPROPERTY(BlueprintReadWrite)
	FRotator StartControllerRotation;

	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> IdleCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> WalkCameraShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* PauseAction;

	UFUNCTION()
	void OnDeathDelegate(const FText& DeathText);

	UFUNCTION()
	void OnRestartDelegate();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void JumpTrigger();

	void PauseGame();

	UFUNCTION()
	void TimeCounter(float DeltaSeconds) const;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAlive;

	//Restrict the player from using mouse input and look (DOES NOT LOCK UI INPUT)
	UPROPERTY(BlueprintReadWrite)
	bool bRestrictMouse;

	//Restrict the player from using keyboard input (DOES NOT LOCK UI INPUT)
	UPROPERTY(BlueprintReadWrite)
	bool bRestrictKeyboard;

	UPROPERTY(BlueprintReadWrite)
	ARTempCamera* Temp_Camera;

	UPROPERTY()
	float SavedMaxAcceleration;

	UPROPERTY(BlueprintReadWrite)
	FVector StartCameraRelativeLocation;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathLocation;

	UFUNCTION(BlueprintImplementableEvent)
	void DrownPlayer(FVector HitLocation);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayerFall(FVector Impulse);

	UFUNCTION(BlueprintImplementableEvent)
	void Respawn();

	UFUNCTION()
	void RestartMenu();

	UFUNCTION()
	void CameraShake() const;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath(EKillerTrap TrapEnum);
};