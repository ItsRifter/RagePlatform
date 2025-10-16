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
	class USkeletalMeshComponent* SkeletalMesh;

	virtual void OnRestartDelegate() override;

	virtual void OnDeathDelegate(const FText& DeathText) override;
};
