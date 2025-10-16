// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/RageCharacter.h"
#include "Player/RTempCamera.h"
#include "Trap.h"

// Sets default values
ATrap::ATrap()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DefaultSceneRoot = CreateDefaultSubobject<UStaticMeshComponent>("Root");
	SetRootComponent(DefaultSceneRoot);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("Triggerbox");
	TriggerBox->SetupAttachment(DefaultSceneRoot);

	KillBox = CreateDefaultSubobject<UBoxComponent>("KillBox");
	KillBox->SetupAttachment(DefaultSceneRoot);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
	AudioComponent->SetupAttachment(DefaultSceneRoot);

	KillText = FText::FromString(TEXT("You died"));
	
	SocketName = "";

	ReactivationTime = 1.0f;
	HoldBeforeReset = 1.0f;

	ImpactVelocity = FVector(0.0f, 0.0f, -50.0f);
}

void ATrap::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();

	bIsTrapReady = true;
	bDoPlayerKill = false;

	if (LoopSound)
	{
		AudioComponent->SetSound(LoopSound);
		AudioComponent->Play();
	}

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATrap::TrapOverlap);
	KillBox->OnComponentBeginOverlap.AddDynamic(this, &ATrap::KillBoxOverlap);
	KillBox->OnComponentEndOverlap.AddDynamic(this, &ATrap::KillBoxLeave);

	GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnDeath.AddDynamic(this, &ATrap::OnDeathDelegate);
		GameInstance->OnGameRestart.AddDynamic(this, &ATrap::OnRestartDelegate);
	}
}

void ATrap::StartTrap()
{
	//Trap is currently activated
	if (!bIsTrapReady)
	{
		return;
	}

	bIsTrapReady = false;
	bDoPlayerKill = true;

	if (OverlappingPlayer)
	{
		KillPlayer(OverlappingPlayer);
		OverlappingPlayer = nullptr;
	}

	GetWorld()->GetTimerManager().SetTimer(TrapActiveHandle, this,
		&ATrap::TrapRetract, HoldBeforeReset, false);
}

void ATrap::TrapRetract()
{
	if (ReactivationTime > 0.0f)
	{
		OnTrapReset(false);
		bDoPlayerKill = false;

		GetWorld()->GetTimerManager().SetTimer(TrapResetHandle, this,
			&ATrap::TrapReset, ReactivationTime, false);
	}
	else 
	{
		TrapReset();
		bDoPlayerKill = false;
		OnTrapReset(false);
	}
}

void ATrap::TrapReset()
{
	//Trap is already on for activation
	if (bIsTrapReady)
	{
		return;
	}

	bIsTrapReady = true;
}

void ATrap::TrapOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsTrapReady) return;

	ARageCharacter* player = Cast<ARageCharacter>(OtherActor);

	if (player)
	{
		StartTrap();
		OnTrapOverlap(player);
	}
}

void ATrap::KillBoxOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	ARageCharacter* player = Cast<ARageCharacter>(OtherActor);

	//Trap hasn't yet activated
	if (!bDoPlayerKill)
	{
		OverlappingPlayer = player;
		return;
	}

	if (player)
	{
		KillPlayer(player);
	}
}

void ATrap::KillBoxLeave(class UPrimitiveComponent* OverlappedComp, 
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlappingPlayer)
	{
		OverlappingPlayer = nullptr;
	}
}

void ATrap::OnDeathDelegate(const FText& DeathText)
{
}

void ATrap::OnRestartDelegate()
{
	GetWorldTimerManager().ClearTimer(TrapActiveHandle);
	GetWorldTimerManager().ClearTimer(TrapResetHandle);

	OnTrapReset(true);
	TrapReset();
}

void ATrap::KillPlayer(ARageCharacter* Player)
{
	if (!Player->bIsAlive)
	{
		return;
	}

	UGameplayStatics::PlaySound2D(this, KillSound);

	GameInstance->OnDeath.Broadcast(KillText);
	
	Player->bIsAlive = false;
}