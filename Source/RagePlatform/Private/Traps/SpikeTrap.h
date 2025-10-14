// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Traps/Trap.h"
#include "SpikeTrap.generated.h"

/**
 * 
 */
UCLASS()
class ASpikeTrap : public ATrap
{
	GENERATED_BODY()
	
public:
	ASpikeTrap();

protected:
	virtual void BeginPlay() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USkeletalMeshComponent* SkeletalMesh;

	/*Make this spike trap activated by a timer*/
	UPROPERTY(EditInstanceOnly)
	bool bOnTimer;

	/*The time to activate this trap | IGNORED IF RANDOM TIME IS ON*/
	UPROPERTY(EditInstanceOnly, meta = (EditCondition = "bOnTimer"))
	float TimeToActivate;

	/*Make the timer value randomised between a min and max*/
	UPROPERTY(EditInstanceOnly, meta = (EditCondition = "bOnTimer"))
	bool bRandomTime;

	/*Minimum time to activate - used with random time*/
	UPROPERTY(EditInstanceOnly, meta = (EditCondition = "bRandomTime"))
	float MinTime;

	/*Maximum time to activate - used with random time*/
	UPROPERTY(EditInstanceOnly, meta = (EditCondition = "bRandomTime"))
	float MaxTime;

	FTimerHandle SpikeTimerHandle;

	UFUNCTION()
	void ResetTriggerTimer();

	UFUNCTION(BlueprintImplementableEvent, Category = "Trap Logic")
	void OnSpikeActivate();

	virtual void StartTrap() override;
	virtual void TrapRetract() override;
	virtual void TrapReset() override;

	virtual void OnRestartDelegate() override;

	virtual void OnDeathDelegate(const FText& DeathText) override;
};
