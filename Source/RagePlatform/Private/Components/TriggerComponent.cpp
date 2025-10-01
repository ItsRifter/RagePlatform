// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Player/RageCharacter.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	StartActive = true;
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::StartTrap()
{
	IsTrapReady = false;

	GetWorld()->GetTimerManager().SetTimer(TrapActiveHandle, this,
		&UTriggerComponent::TrapRetract, HoldBeforeReset, false);
}

void UTriggerComponent::TrapRetract()
{
	if (ReactivationTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(TrapResetHandle, this,
			&UTriggerComponent::TrapReset, ReactivationTime, false);
	}
	else
	{
		TrapReset();
	}
}

void UTriggerComponent::TrapReset()
{
	OnTrapReset();
	IsTrapReady = true;
}

void UTriggerComponent::TrapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsTrapReady) return;

	ARageCharacter* player = Cast<ARageCharacter>(OtherActor);

	if (player)
	{
		StartTrap();
		OnTrapOverlap(player);
	}
}