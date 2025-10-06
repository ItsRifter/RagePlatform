// Fill out your copyright notice in the Description page of Project Settings.

#include "Gameplay/RagePlatState.h"
#include <Kismet/GameplayStatics.h>

void ARagePlatState::DeleteActorByTag(FName Tag)
{
    if (Tag == NAME_None)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attempted to delete actor with invalid tag."));
        return;
    }

    TArray<AActor*> FoundActors;
   
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, FoundActors);

    if (FoundActors.Num() > 0)
    {
        AActor* TargetActor = FoundActors[0];

        TargetActor->Destroy();
    }
}