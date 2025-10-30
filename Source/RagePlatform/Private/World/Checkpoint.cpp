// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Framework/RGameInstance.h"
#include "Gameplay/RagePlatGame.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RageCharacter.h"
#include "World/Checkpoint.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    Triggerbox = CreateDefaultSubobject<UBoxComponent>("Triggerbox");
    Triggerbox->SetupAttachment(GetRootComponent());
    Triggerbox->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
    Triggerbox->SetBoxExtent(FVector(50.0f));

    RespawnOrientator = CreateDefaultSubobject<UArrowComponent>("SpawnRotation");
    RespawnOrientator->SetupAttachment(Triggerbox);
    RespawnOrientator->ArrowLength = 64.0f;
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	Triggerbox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlapBegin);
    
    if (URGameInstance* GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
    {
        if (!GameInstance->bIsEasyMode)
        {
            SetActorHiddenInGame(true);
            SetActorEnableCollision(false);
            Triggerbox->SetActive(false);
        }
    }
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

void ACheckpoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (Cast<ARageCharacter>(OtherActor))
    {
        if (bHasActivated)
        {
            return;
        }

        bHasActivated = true;

		ARagePlatGame* Gamemode = Cast<ARagePlatGame>(UGameplayStatics::GetGameMode(GetWorld()));

        if (Gamemode)
        {
            Gamemode->SetCheckpoint(this);
			OnCheckpointActivation();
        }
    }
}

FRotator ACheckpoint::GetSpawnRotation()
{
    return RespawnOrientator->GetComponentRotation();
}