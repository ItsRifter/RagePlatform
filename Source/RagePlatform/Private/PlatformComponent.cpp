// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformComponent.h"
#include "Components/InterpToMovementComponent.h"
#include "Components/BoxComponent.h"
#include "PlatformPath.h"

// Sets default values for this component's properties
UPlatformComponent::UPlatformComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MoveOnStart = false;
	MoveDuration = 5.0f;

	MoveComponent = CreateDefaultSubobject<UInterpToMovementComponent>("Mover");
	MoveComponent->Duration = MoveDuration;
	MoveComponent->bSweep = true;
	MoveComponent->BehaviourType = EInterpToBehaviourType::Loop_Reset;
}

// Called when the game starts
void UPlatformComponent::BeginPlay()
{
	Super::BeginPlay();

	NodeIndex = 0;

	if (Paths.Num() <= 1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, GetOwner()->GetActorNameOrLabel() + " has only 1 or no paths set.");
		}

		return;
	}

	APlatformPath* startPath = Paths[0];

	GetOwner()->SetActorLocation(startPath->GetActorLocation());

	for (APlatformPath* path : Paths)
	{
		MoveComponent->AddControlPointPosition(path->GetActorLocation(), false);
	}

	if (MoveOnStart)
	{
		StartMoving();
	}
	else
	{
		MoveComponent->Deactivate();
	}
}


void UPlatformComponent::StartMoving()
{
	if (IsMoving) return;

	MoveComponent->Activate();
	IsMoving = true;
}

void UPlatformComponent::StopMoving()
{
	if (!IsMoving) return;

	MoveComponent->Deactivate();
	IsMoving = false;
}

// Called every frame
void UPlatformComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

