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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText DeathMessage;

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

	UPROPERTY(EditDefaultsOnly)
	USoundBase* ActivateSound;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* LoopSound;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* ResetSound;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* KillSound;

	UPROPERTY()
	bool bIsTrapReady;

	UPROPERTY()
	bool bDoPlayerKill;

	UPROPERTY(EditDefaultsOnly)
	FName SocketName;

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
};
