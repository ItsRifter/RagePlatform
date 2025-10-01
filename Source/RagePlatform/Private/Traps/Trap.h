// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

class UPrimitiveComponent;
class AActor;
class UBoxComponent;
class ARageCharacter;

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

	UPROPERTY(EditAnywhere)
	bool DisableTrigger;

	UPROPERTY(EditAnywhere)
	bool DisableKill;

	UPROPERTY(EditAnywhere)
	bool StartReady;

	bool IsTrapReady;

	UFUNCTION()
	void StartTrap();

	UFUNCTION()
	void TrapReset();

	UFUNCTION(BlueprintImplementableEvent, Category = "Trap Logic")
	void OnTrapReset();

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

	UFUNCTION(BlueprintImplementableEvent, Category = "Trap Logic")
	void OnTrapOverlap(ARageCharacter* player);

	FTimerHandle TrapActiveHandle;
	FTimerHandle TrapResetHandle;
};
