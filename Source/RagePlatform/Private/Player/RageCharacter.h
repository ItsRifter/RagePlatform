// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RageCharacter.generated.h"

class URRestartWidget;
class UInputAction;
class UCameraComponent;
class UInputMappingContext;
class URGameInstance;
struct FInputActionValue;

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

	UPROPERTY(EditDefaultsOnly, Category = "Wigets")
	TSubclassOf<UUserWidget> RestartWidgetBP;

	UPROPERTY()
	UUserWidget* RestartWidget;

	UPROPERTY()
	URRestartWidget* RestartWidgetRef;

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
	UInputAction* CrouchAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* PauseAction;

	UFUNCTION()
	void OnDeathDelegate(const FText& DeathText);

	UFUNCTION()
	void OnRestartDelegate();

	void Move(const FInputActionValue& Value);
	void Jump();
	void PauseGame();
	void Look(const FInputActionValue& Value);
	void DoCrouch();
	void Standup();

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAlive;

	UPROPERTY(BlueprintReadWrite)
	bool bRestarted;
	
	UPROPERTY()
	float SavedMaxAcceleration;

	UPROPERTY(BlueprintReadWrite)
	FVector StartCameraRelativeLocation;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathLocation;
	
	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();

	UFUNCTION(BlueprintImplementableEvent)
	void DrownPlayer(FVector HitLocation);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerFall();

	UFUNCTION()
	void RestartMenu() const;

	UFUNCTION()
	void CameraShake();
};
