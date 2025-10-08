// Fill out your copyright notice in the Description page of Project Settings.


#include "RExplosionsBase.h"

#include "Components/SphereComponent.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
	KillText = FText::FromString(TEXT("Something You Touched Exploded!!"));
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
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                      bool bFromSweep, const FHitResult& SweepResult)
{
	if (bExploded)
	{
		return;
	}

	if (ARageCharacter* PlayerCharacter = Cast<ARageCharacter>(OtherActor))
	{
		if (ExplosionSound)
		{
			UGameplayStatics::PlaySound2D(this, ExplosionSound);
		}
		if (ExplosionParticleSystem)
		{
			ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(
				this, ExplosionParticleSystem, GetActorLocation());
		}

		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
			PlayerCharacter->GetActorLocation(),
			GetActorLocation());
		const FVector LaunchVelocity = LookAtRotation.Vector() * -750.f;

		PlayerCharacter->LaunchCharacter(
			FVector(LaunchVelocity.X, LaunchVelocity.Y, 250.f),
			true,
			true);

		StaticMesh->SetVisibility(false);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DeathOverlap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GameInstance->OnDeath.Broadcast(KillText);
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
