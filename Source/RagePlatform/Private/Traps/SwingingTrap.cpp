#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Traps/SwingingTrap.h"

ASwingingTrap::ASwingingTrap()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	SkeletalMesh->SetupAttachment(GetRootComponent());

	SocketName = "";
	KillBox->SetupAttachment(SkeletalMesh, SocketName);
}

void ASwingingTrap::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	KillBox->SetupAttachment(SkeletalMesh, SocketName);
}

void ASwingingTrap::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->Deactivate();

	if (SocketName != NAME_None)
	{
		KillBox->AttachToComponent(SkeletalMesh, 
			FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}
}

void ASwingingTrap::OnRestartDelegate()
{
}

void ASwingingTrap::OnDeathDelegate(const FText& DeathText)
{
}
