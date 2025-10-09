// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

class UPrimitiveComponent;
class AActor;
class UBoxComponent;
class ARageCharacter;
class USoundBase;

struct FHitResult;

UCLASS(Blueprintable)
class ATrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere)
	float ReactivationTime;

	UPROPERTY(EditAnywhere)
	float HoldBeforeReset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* TriggerBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* KillBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* KillAttachment;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* TrapMesh;

	UPROPERTY()
	URGameInstance* GameInstance;

	UPROPERTY(BlueprintReadWrite)
	FVector StartLocation;

	UPROPERTY(BlueprintReadWrite)
	FRotator StartRotation;

	UPROPERTY(EditDefaultsOnly)
	FText KillText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* ActivateSound;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* LoopSound;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* ResetSound;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* KillSound;

	bool bIsTrapReady;

	UFUNCTION()
	virtual void StartTrap();

	UFUNCTION()
	virtual void TrapReset();

	/*Called when player is being reset*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Trap Logic")
	void OnTrapReset();

	/*Called when trap was tripped by the player*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Trap Logic")
	void OnTrapOverlap(ARageCharacter* player);

	/*Called when trap has managed to kill the player*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Trap Logic")
	void OnKilledPlayer(ARageCharacter* player);

	UFUNCTION()
	void TrapRetract();

	UFUNCTION()
	void TrapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void KillBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnDeathDelegate(const FText& DeathText);

	UFUNCTION()
	virtual void OnRestartDelegate();

	FTimerHandle TrapActiveHandle;
	FTimerHandle TrapResetHandle;
};
