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

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

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

	UPROPERTY()
	class URGameInstance* GameInstance;

	//Used for traps in cases the player is overlapping the trap but hasn't activated,
	//this is to fix trap not killing in this case.
	UPROPERTY()
	class ARageCharacter* OverlappingPlayer;

	UPROPERTY(BlueprintReadWrite)
	FVector StartLocation;

	UPROPERTY(BlueprintReadWrite)
	FRotator StartRotation;

	UPROPERTY(EditDefaultsOnly, Category = "DeathScreen")
	FText KillText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UAudioComponent* AudioComponent;

	//Sound when trap is activated
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* ActivateSound;

	//Sound when trap is currently active - looping
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* LoopSound;

	//Sound when trap is resetting
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* ResetSound;

	//The sound when killing the player
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundBase* KillSound;

	UPROPERTY()
	bool bIsTrapReady;

	UPROPERTY()
	bool bDoPlayerKill;

	UPROPERTY(EditDefaultsOnly)
	FName SocketName;

	/*The impact velocity to apply when killing the player
	!! SOME TRAPS MAY SET THEIR OWN VALUES !!*/
	UPROPERTY(EditDefaultsOnly)
	FVector ImpactVelocity;

	//The offset to apply for the player camera when looking at this actor on killed
	UPROPERTY(EditDefaultsOnly)
	FVector DeathFocusOffset;

	FTimerHandle TrapActiveHandle;
	FTimerHandle TrapResetHandle;

	UFUNCTION()
	virtual void StartTrap();

	UFUNCTION()
	virtual void TrapReset();

	UFUNCTION()
	virtual void TrapRetract();

	/*Called when trap is being reset | bGameRestart: if game has restarted*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Trap Logic")
	void OnTrapReset(bool bGameRestart);

	/*Called when trap was tripped by the player*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Trap Logic")
	void OnTrapOverlap(ARageCharacter* player);

	/*Called when trap has managed to kill the player*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Trap Logic")
	void OnKilledPlayer(ARageCharacter* player);

	UFUNCTION()
	void TrapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void KillBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void KillBoxLeave(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, 
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnDeathDelegate(const FText& DeathText);

	UFUNCTION()
	virtual void OnRestartDelegate();

	UFUNCTION()
	virtual void KillPlayer(class ARageCharacter* Player);
};
