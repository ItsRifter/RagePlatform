// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/RageCharacter.h"
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

	ReactivationTime = 5.0f;
	HoldBeforeReset = 1.0f;
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

	if (LoopSound)
	{
		AudioComponent->SetSound(LoopSound);
		AudioComponent->Play();
	}

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATrap::TrapOverlap);
	KillBox->OnComponentBeginOverlap.AddDynamic(this, &ATrap::KillBoxOverlap);

	if (KillAttachment)
		KillBox->SetupAttachment(KillAttachment);

	GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnDeath.AddDynamic(this, &ATrap::OnDeathDelegate);
		GameInstance->OnGameRestart.AddDynamic(this, &ATrap::OnRestartDelegate);
	}
}

void ATrap::StartTrap()
{
	bIsTrapReady = false;

	UGameplayStatics::PlaySoundAtLocation(this, ActivateSound, GetActorLocation());

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
	UGameplayStatics::PlaySoundAtLocation(this, ResetSound, GetActorLocation());
	OnTrapReset();
	bIsTrapReady = true;
}

void ATrap::TrapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsTrapReady) return;

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
		UGameplayStatics::PlaySound2D(this, KillSound);

		GameInstance->OnDeath.Broadcast(KillText);
		OnKilledPlayer(player);
	}
}

void ATrap::OnDeathDelegate(const FText& DeathText)
{
}

void ATrap::OnRestartDelegate()
{
}