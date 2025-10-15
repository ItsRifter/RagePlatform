// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Traps/Trap.h"
#include "SawTrap.generated.h"

/**
 * 
 */
UCLASS()
class ASawTrap : public ATrap
{
	GENERATED_BODY()
	
public:
	ASawTrap();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditInstanceOnly, Category = "Trap")
	float MoveDistance;

	virtual void OnDeathDelegate(const FText& DeathText) override;

	virtual void OnRestartDelegate() override;

	virtual void KillPlayer(class ARageCharacter* Player) override;

private:
	class UTimelineComponent* TimelineComponent;

	UFUNCTION()
	void OnTimelineProgress(float val);
};
