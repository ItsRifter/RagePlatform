// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformComponent.h"
#include "Components/InterpToMovementComponent.h"

// Sets default values for this component's properties
UPlatformComponent::UPlatformComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MoveComponent = CreateDefaultSubobject<UInterpToMovementComponent>("Mover");
	MoveComponent->Duration = MoveDuration;
	MoveComponent->bSweep = true;
	MoveComponent->BehaviourType = EInterpToBehaviourType::Loop_Reset;
}


// Called when the game starts
void UPlatformComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UPlatformComponent::StartMoving()
{
}

void UPlatformComponent::StopMoving()
{
}

// Called every frame
void UPlatformComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

