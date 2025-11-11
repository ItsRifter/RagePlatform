// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BoxComponent.h"
#include "Player/RageCharacter.h"
#include "TriggerComponent.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("Triggerbox");

	bStartActive = true;
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	//TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::Trigger);

	if (!bStartActive)
	{
		TriggerBox->Deactivate();
	}
}

void UTriggerComponent::Trigger()
{
	bIsTrapReady = false;

	GetWorld()->GetTimerManager().SetTimer(TrapActiveHandle, this,
		&UTriggerComponent::DoReset, HoldBeforeReset, false);
}

void UTriggerComponent::DoReset()
{
	if (ReactivationTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(TrapResetHandle, this,
			&UTriggerComponent::Reset, ReactivationTime, false);
	}
	else
	{
		Reset();
	}
}

void UTriggerComponent::Reset()
{
	OnReset();
	bIsTrapReady = true;
}

void UTriggerComponent::TriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsTrapReady) return;

	ARageCharacter* player = Cast<ARageCharacter>(OtherActor);

	if (player)
	{
		Trigger();
		OnOverlap(player);
	}
}