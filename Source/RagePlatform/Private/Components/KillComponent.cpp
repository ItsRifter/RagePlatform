// Fill out your copyright notice in the Description page of Project Settings.


#include "KillComponent.h"
#include "Player/RageCharacter.h"

// Sets default values for this component's properties
UKillComponent::UKillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	bStartReady = true;
}

// Called when the game starts
void UKillComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UKillComponent::KillBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARageCharacter* player = Cast<ARageCharacter>(OtherActor);

	if (player)
	{
		player->Death();
	}
}

