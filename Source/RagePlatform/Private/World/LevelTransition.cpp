// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "Gameplay/RagePlatGame.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RageCharacter.h"
#include "World/LevelTransition.h"

// Sets default values
ALevelTransition::ALevelTransition()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DefaultSceneRoot = CreateDefaultSubobject<UStaticMeshComponent>("Root");
	SetRootComponent(DefaultSceneRoot);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	TriggerBox->SetupAttachment(DefaultSceneRoot);
}

// Called when the game starts or when spawned
void ALevelTransition::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransition::OnLevelTrigger);
}

void ALevelTransition::OnLevelTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (NextLevelName.Len() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing level name to transition to"));
		return;
	}

	ARageCharacter* Player = Cast<ARageCharacter>(OtherActor);

	if(Player)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(NextLevelName));
	}
}