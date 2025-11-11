// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Traps/SpikeTrap.h"

ASpikeTrap::ASpikeTrap()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	SkeletalMesh->SetupAttachment(GetRootComponent());

	KillBox->SetupAttachment(SkeletalMesh, SocketName);

	bOnTimer = false;
	TimeToActivate = 2.0f;

	bRandomTime = false;
	MinTime = 1.0f;
	MaxTime = 2.0f;
}

void ASpikeTrap::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	KillBox->SetupAttachment(SkeletalMesh, SocketName);
}

void ASpikeTrap::BeginPlay()
{
	Super::BeginPlay();

	if (bOnTimer)
	{
		TriggerBox->OnComponentBeginOverlap.RemoveDynamic(this, &ASpikeTrap::TrapOverlap);

		ResetTriggerTimer();
	}
}

void ASpikeTrap::ResetTriggerTimer()
{
	float Time = TimeToActivate;

	if (bRandomTime)
	{
		Time = FMath::RandRange(MinTime, MaxTime);
	}

	bIsTrapReady = true;

	GetWorld()->GetTimerManager().SetTimer(SpikeTimerHandle, this,
		&ASpikeTrap::StartTrap, Time, false);
}

void ASpikeTrap::StartTrap()
{
	SkeletalMesh->SetVisibility(true, true);

	if (bOnTimer)
	{
		OnSpikeActivate();
	}

	Super::StartTrap();
}

void ASpikeTrap::TrapRetract()
{
	Super::TrapRetract();
}

void ASpikeTrap::TrapReset()
{
	SkeletalMesh->SetVisibility(false, true);

	Super::TrapReset();

	if (bOnTimer)
	{
		ResetTriggerTimer();
	}
}

void ASpikeTrap::OnRestartDelegate()
{
	if (bOnTimer)
	{
		GetWorldTimerManager().ClearTimer(SpikeTimerHandle);
	}

	bDoPlayerKill = false;
	TrapReset();

	Super::OnRestartDelegate();
}

void ASpikeTrap::OnDeathDelegate(const FText& DeathText)
{
}