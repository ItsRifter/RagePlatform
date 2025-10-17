// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Traps/Trap.h"
#include "SwingingTrap.generated.h"

UCLASS(Blueprintable)
class ASwingingTrap : public ATrap
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASwingingTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* SwingPivot;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditInstanceOnly, Category = "Trap", meta = (ClampMin = 0.1f, ClampMax = 5.0f))
	float SwingSpeed;

	//UPROPERTY()
	//float SwingAngle;

	virtual void OnRestartDelegate() override;

	virtual void OnDeathDelegate(const FText& DeathText) override;

	virtual void KillPlayer(class ARageCharacter* Player) override;

private:
	UPROPERTY()
	class UTimelineComponent* TimelineComponent;

	bool bGoReverse;

	bool bSwingingLeft;
	float LastVal;

	UFUNCTION()
	void OnTimelineProgress(float val);
};
