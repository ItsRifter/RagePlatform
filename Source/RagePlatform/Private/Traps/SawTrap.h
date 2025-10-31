// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Traps/Trap.h"
#include "SawTrap.generated.h"

class UAnimationAsset;

UCLASS()
class ASawTrap : public ATrap
{
	GENERATED_BODY()
	
public:
	ASawTrap();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditInstanceOnly, Category = "Trap")
	float MoveDistance;

	//Multiplies the move rate
	UPROPERTY(EditInstanceOnly, Category = "Trap", meta = (ClampMin = 0.1f, ClampMax = 3.0f))
	float SpeedMultiplier;

	virtual void OnDeathDelegate(const FText& DeathText) override;

	virtual void OnRestartDelegate() override;

	virtual void KillPlayer(class ARageCharacter* Player) override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY()
	TObjectPtr<class UArrowComponent> DistanceVisualizer;

	//Fired when saw trap is going forward (not in reverse)
	UFUNCTION(BlueprintImplementableEvent)
	void OnForwardDirection();

	//Fired when saw trap is in reverse
	UFUNCTION(BlueprintImplementableEvent)
	void OnReverseDirection();

private:
	class UTimelineComponent* TimelineComponent;

	UPROPERTY()
	bool bIsReversed;

	UFUNCTION()
	void OnTimelineProgress(float val);

	UFUNCTION()
	void OnTimelineFinish();
};
