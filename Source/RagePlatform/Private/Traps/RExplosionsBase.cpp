// Fill out your copyright notice in the Description page of Project Settings.


#include "RExplosionsBase.h"
#include "TrapEnums.h"
#include "Components/BoxComponent.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/RageCharacter.h"
#include "Player/RTempCamera.h"

// Sets default values
ARExplosionsBase::ARExplosionsBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(GetRootComponent());

	DeathOverlap = CreateDefaultSubobject<UBoxComponent>("DeathOverlap");
	DeathOverlap->SetupAttachment(GetRootComponent());

	ExplosionSound = nullptr;
	ExplosionParticleSystem = nullptr;
	KillTexts.Add(FText::FromString(TEXT("Something You Touched Exploded!!")));
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
	if (ARageCharacter* PlayerCharacter = Cast<ARageCharacter>(OtherActor))
	{
		if (!PlayerCharacter->bIsAlive)
		{
			return;
		}
		
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

		const FVector LaunchVelocity = LookAtRotation.Vector() * -450.0f;

		/*PlayerCharacter->LaunchCharacter(
			FVector(LaunchVelocity.X, LaunchVelocity.Y, 250.f),
			true,
			true);*/

		PlayerCharacter->PlayerFall(FVector(LaunchVelocity.X, LaunchVelocity.Y, 200.0f));

		if (PlayerCharacter->Temp_Camera)
		{
			PlayerCharacter->Temp_Camera->FocusVar = this;
			PlayerCharacter->Temp_Camera->StartFocus();
		}

		StaticMesh->SetVisibility(false);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DeathOverlap->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		const int32 Index = UKismetMathLibrary::RandomIntegerInRange(0,KillTexts.Num() - 1);
		GameInstance->OnDeath.Broadcast(KillTexts[Index]);
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
}
