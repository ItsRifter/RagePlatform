// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Checkpoint.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//CheckpointIndex = 0;
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACheckpoint::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    // Only assign and increment if we are running in the game world
    /*if (GetWorld() && GetWorld()->IsGameWorld())
    {
        CheckpointIndex = NextCheckpointIndex;

        NextCheckpointIndex++;

        UE_LOG(LogTemp, Warning, TEXT("Runtime Checkpoint assigned index: %d"), CheckpointIndex);
    }*/
}

//#if WITH_EDITOR
//void ACheckpoint::OnConstruction(const FTransform& Transform)
//{
//    Super::OnConstruction(Transform);
//
//    TArray<AActor*> FoundActors;
//    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpoint::StaticClass(), FoundActors);
//
//    CheckpointIndex = FoundActors.Num();
//
//    UE_LOG(LogTemp, Warning, TEXT("Editor Checkpoint assigned index: %d"), CheckpointIndex);
//}
//#endif

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

