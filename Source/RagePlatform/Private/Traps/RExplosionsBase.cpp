// Fill out your copyright notice in the Description page of Project Settings.


#include "RExplosionsBase.h"

#include "Components/SphereComponent.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/RageCharacter.h"


// Sets default values
ARExplosionsBase::ARExplosionsBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(GetRootComponent());

	DeathOverlap = CreateDefaultSubobject<USphereComponent>("DeathOverlap");
	DeathOverlap->SetupAttachment(GetRootComponent());

	ExplosionSound = nullptr;
	ExplosionParticleSystem = nullptr;
	bExploded = false;
}

// Called when the game starts or when spawned
void ARExplosionsBase::BeginPlay()
{
	Super::BeginPlay();

	DeathOverlap->OnComponentBeginOverlap.AddDynamic(this, &ARExplosionsBase::OnComponentBeginOverlapKillBox);
	GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(this));
	GameInstance->OnGameRestart.AddDynamic(this, &ARExplosionsBase::OnRestartDelegate);
}

void ARExplosionsBase::OnComponentBeginOverlapKillBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bExploded)
	{
		return;
	}
	
	if (Cast<ARageCharacter>(OtherActor))
	{
		if (ExplosionSound)
		{
			UGameplayStatics::PlaySound2D(this,ExplosionSound);
		}
		if (ExplosionParticleSystem)
		{
			ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(this,ExplosionParticleSystem,GetActorLocation());
		}

		StaticMesh->SetVisibility(false);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DeathOverlap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GameInstance->OnDeath.Broadcast();
		bExploded = true;
	}
}

void ARExplosionsBase::OnRestartDelegate()
{
	if (ParticleSystemComponent)
	{
		ParticleSystemComponent->DestroyComponent();
	}
	StaticMesh->SetVisibility(true);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DeathOverlap->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bExploded = false;
}

