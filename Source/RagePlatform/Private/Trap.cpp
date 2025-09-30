// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "RageCharacter.h"
#include "Trap.h"

// Sets default values
ATrap::ATrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<UStaticMeshComponent>("Root");
	SetRootComponent(DefaultSceneRoot);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("Triggerbox");
	TriggerBox->SetupAttachment(DefaultSceneRoot);

	KillBox = CreateDefaultSubobject<UBoxComponent>("KillBox");
	KillBox->SetupAttachment(DefaultSceneRoot);

	StartReady = true;

	ReactivationTime = 5.0f;
	HoldBeforeReset = 1.0f;
}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();

	IsTrapReady = StartReady;

	if(!DisableTrigger)
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATrap::TrapOverlap);

	if(!DisableKill)
		KillBox->OnComponentBeginOverlap.AddDynamic(this, &ATrap::KillBoxOverlap);

	if (KillAttachment)
		KillBox->SetupAttachment(KillAttachment);

	if (DisableTrigger && DisableKill)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, GetOwner()->GetActorNameOrLabel() + " has both trigger and kill boxes disabled");
		}
	}
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrap::StartTrap()
{
	IsTrapReady = false;

	GetWorld()->GetTimerManager().SetTimer(TrapActiveHandle, this,
		&ATrap::TrapRetract, HoldBeforeReset, false);
}

void ATrap::TrapRetract()
{
	if (ReactivationTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(TrapResetHandle, this,
			&ATrap::TrapReset, ReactivationTime, false);
	}
	else 
	{
		TrapReset();
	}
}

void ATrap::TrapReset()
{
	OnTrapReset();
	IsTrapReady = true;
}

void ATrap::TrapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsTrapReady) return;

	ARageCharacter* player = Cast<ARageCharacter>(OtherActor);

	if (player)
	{
		StartTrap();
		OnTrapOverlap(player);
	}
}

void ATrap::KillBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARageCharacter* player = Cast<ARageCharacter>(OtherActor);

	if (player)
	{
		player->Death();
	}
}