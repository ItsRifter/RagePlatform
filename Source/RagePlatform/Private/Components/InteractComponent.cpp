// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

    InteractType = EInteractHandle::SetActorVisibility;
    DeleteOnUse = false;
}

// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

    UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetFindTag, FoundActors);
}

void UInteractComponent::Use()
{
    if (TargetFindTag != NAME_None)
    {
        if (InteractType == EInteractHandle::DeleteActor)
        {
            for (AActor* CurActor : FoundActors)
            {
                UE_LOG(LogTemp, Display, TEXT("Deleting %s"), *CurActor->GetActorNameOrLabel());
                CurActor->Destroy();
            }

            //Empty after deleting - prevent nullptr references
            if (!FoundActors.IsEmpty())
            {
                FoundActors.Empty();
            }
        }     

        if (InteractType == EInteractHandle::SetActorVisibility)
        {
            for(AActor* CurActor : FoundActors)
            {
                UE_LOG(LogTemp, Display, TEXT("Toggling hidden to %s"), *CurActor->GetActorNameOrLabel());
                CurActor->SetActorHiddenInGame(!Toggle);
                CurActor->SetActorEnableCollision(Toggle);

                //Don't toggle tick if its never meant to
                if(CurActor->CanEverTick())
                    CurActor->SetActorTickEnabled(Toggle);
            }
        }

        if (InteractType == EInteractHandle::ToggleComponent)
        {
            for (AActor* CurActor : FoundActors)
            {
                TArray<UActorComponent*> Components = CurActor->GetComponents().Array();
                for (UActorComponent* Comp : Components)
                {
                    if (Comp && Comp->ComponentHasTag(TargetFindTag))
                    {
                        UE_LOG(LogTemp, Display, TEXT("Toggling component %s in %s"), 
                            *Comp->GetName(), *CurActor->GetActorNameOrLabel());

                        if (Toggle)
                        {
                            Comp->Activate();
                        }
                        else
                        {
                            Comp->Deactivate();
                        }
                    }
                }
            }      
        }
    }

    if (DeleteOnUse)
    {
	    AActor* owner = GetOwner();

	    if (owner)
	    {
		    owner->Destroy();
	    }
    }
}
