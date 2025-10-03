// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BoxComponent.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/RageCharacter.h"
#include "Traps/RChandelier.h"

// Sets default values
ARChandelier::ARChandelier()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot"));

	ChandelierMesh = CreateDefaultSubobject<UStaticMeshComponent>("ChandelierMesh");
	ChandelierMesh->SetupAttachment(GetRootComponent());

	KillBox = CreateDefaultSubobject<UBoxComponent>("KillBox");
	KillBox->SetupAttachment(ChandelierMesh);

	PlayerOverlapBox = CreateDefaultSubobject<UBoxComponent>("PlayerOverlap");
	PlayerOverlapBox->SetupAttachment(GetRootComponent());

	FallSpeed = 7;
}

// Called when the game starts or when spawned
void ARChandelier::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ARageCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnDeath.AddDynamic(this, &ARChandelier::OnDeathDelegate);
		GameInstance->OnGameRestart.AddDynamic(this,&ARChandelier::OnRestartDelegate);
	}

	KillBox->OnComponentBeginOverlap.AddDynamic(this, &ARChandelier::OnComponentBeginOverlapKillBox);
	PlayerOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ARChandelier::OnComponentBeginOverlapPlayerBox);

	ChandelierMesh->SetSimulatePhysics(false);

	StartLocation = ChandelierMesh->GetComponentLocation();
	StartRotation = ChandelierMesh->GetComponentRotation();
}

void ARChandelier::OnComponentBeginOverlapKillBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
	if (Cast<ARageCharacter>(OtherActor))
	{
		GameInstance->OnDeath.Broadcast();
	}
}

void ARChandelier::OnComponentBeginOverlapPlayerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                    bool bFromSweep, const FHitResult& SweepResult)
{
	if (bChandelierFell)
	{
		return;
	}

	if (Cast<ARageCharacter>(OtherActor))
	{
		ChandelierMesh->SetSimulatePhysics(true);

		const float ImpulseZ = UKismetMathLibrary::MapRangeClamped(FallSpeed, 1.f, 10.f, 500.f, 1500.f) * -1;
		ChandelierMesh->AddImpulse(FVector(0.f, 0.f, ImpulseZ), NAME_None, true);
		bChandelierFell = true;
	}
}

void ARChandelier::OnDeathDelegate()
{
	bChandelierFell = false;
}

void ARChandelier::OnRestartDelegate()
{
	ChandelierMesh->SetSimulatePhysics(false);
	ChandelierMesh->SetWorldLocation(StartLocation);
	ChandelierMesh->SetWorldRotation(StartRotation);
}
