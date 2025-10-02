// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BoxComponent.h"
#include "Components/InteractComponent.h"
#include "Gameplay/RagePlatState.h"
#include "Player/RageCharacter.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UInteractComponent::Obtain()
{
    if (TargetDeletionTag != NAME_None)
    {
        if (AGameStateBase* GS = GetWorld()->GetGameState())
        {
            // You'll need to cast to your custom GameState to access the function below
            if (ARagePlatState* CustomGS = Cast<ARagePlatState>(GS))
            {
                CustomGS->DeleteActorByTag(TargetDeletionTag);
            }
        }
    }

	AActor* owner = GetOwner();

	if (owner)
	{
		owner->Destroy();
	}
}
