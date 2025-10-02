// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RageCharacter.generated.h"

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

	UPROPERTY(EditDefaultsOnly, Category = "Wigets")
	TSubclassOf<UUserWidget> RestartWidgetBP;

	UPROPERTY()
	UUserWidget* RestartWidget;

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

	/// <summary>
	/// TODO: Remove
	/// </summary>

	UPROPERTY()
	bool IsAlive;

	UFUNCTION()
	void OnDeathDelegate(bool bIsDead);

	void Move(const FInputActionValue& Value);
	void Jump();
	void Look(const FInputActionValue& Value);
	void DoCrouch();
	void Standup();

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// TODO: Remove
	/// </summary>

	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION()
	void Respawn();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();

	UFUNCTION(BlueprintImplementableEvent)
	void OnRespawn();

	UFUNCTION()
	void RestartMenu();
};
