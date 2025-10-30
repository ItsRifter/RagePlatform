// Fill out your copyright notice in the Description page of Project Settings.


#include "RSpikes.h"

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/RageCharacter.h"
#include "Player/RTempCamera.h"


// Sets default values
ARSpikes::ARSpikes()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot"));

	SpikeMesh = CreateDefaultSubobject<UStaticMeshComponent>("SpikeMesh");
	SpikeMesh->SetupAttachment(GetRootComponent());
	SpikeMesh->SetRelativeLocation(FVector(0.f, 0.f, -100.f));

	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	OverlapBox->SetupAttachment(GetRootComponent());
	OverlapBox->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	OverlapBox->SetBoxExtent(FVector(50.f));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
	AudioComponent->SetupAttachment(GetRootComponent());

	SpikeSound = nullptr;
	KillSound = nullptr;

	bSpikeResetComplete = true;
	PlayerImpact = FVector::ZeroVector;
	KillTexts.Add(FText::FromString(TEXT("You were Spiked!!")));
	SpikeUpDuration = 0.05f;
	SpikeDownDuration = 1.f;
}

// Called when the game starts or when spawned
void ARSpikes::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(this));
	PlayerCharacter = Cast<ARageCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (GameInstance)
	{
		GameInstance->OnGameRestart.AddDynamic(this, &ARSpikes::OnRestartDelegate);
	}

	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ARSpikes::OnComponentBeginOverlap);
}

void ARSpikes::OnRestartDelegate()
{
	bSpikeResetComplete = true;
	
	StopSpike();
	SpikeMesh->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
	
	if (PlayerCharacter->Temp_Camera)
	{
		PlayerCharacter->Temp_Camera->StopFocus();
	}
}

void ARSpikes::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (Cast<ARageCharacter>(OtherActor))
	{
		if (bSpikeResetComplete && PlayerCharacter->bIsAlive)
		{
			PlayerCharacter->bIsAlive = false;
			bSpikeResetComplete = false;
			OnPlayerKill();

			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
			PlayerCharacter->GetActorLocation(),
			GetActorLocation());
			const FVector LaunchVelocity = LookAtRotation.Vector() * -200.f;
			PlayerImpact = FVector(LaunchVelocity.X,LaunchVelocity.Y,250.f);
			
			PlayerCharacter->PlayerFall(PlayerImpact);

			if (PlayerCharacter->Temp_Camera)
			{
				PlayerCharacter->Temp_Camera->FocusVar = this;
				PlayerCharacter->Temp_Camera->StartFocus();
			}			

			if (KillSound)
			{
				AudioComponent->SetSound(KillSound);
				AudioComponent->Play();
			}

			constexpr EMoveComponentAction::Type MoveAction = EMoveComponentAction::Type::Move;
			LatentInfoSpikeUP.CallbackTarget = this;
			LatentInfoSpikeUP.UUID = 2;
			LatentInfoSpikeUP.Linkage = 0;
			LatentInfoSpikeUP.ExecutionFunction = FName("OnMoveFinished");

			UKismetSystemLibrary::MoveComponentTo(
				SpikeMesh,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				true,
				true,
				SpikeUpDuration,
				false,
				MoveAction,
				LatentInfoSpikeUP);
		}
	}
}

void ARSpikes::OnMoveFinished()
{
	const int32 Index = UKismetMathLibrary::RandomIntegerInRange(0,KillTexts.Num() - 1);
	GameInstance->OnDeath.Broadcast(KillTexts[Index]);
	
	GetWorld()->GetTimerManager().SetTimer(ReverseTimerHandle, this, &ARSpikes::ReverseSpike, 2.f, false);
}

void ARSpikes::ReverseSpike()
{
	constexpr EMoveComponentAction::Type MoveAction = EMoveComponentAction::Type::Move;
	LatentInfoSpikeDown.CallbackTarget = this;
	LatentInfoSpikeDown.UUID = 1;
	LatentInfoSpikeDown.Linkage = 0;
	LatentInfoSpikeDown.ExecutionFunction = FName("OnMoveFinishedDown");

	UKismetSystemLibrary::MoveComponentTo(
		SpikeMesh,
		FVector(0.f,0.f,-100.f),
		FRotator::ZeroRotator,
		true,
		true,
		SpikeDownDuration,
		false,
		MoveAction,
		LatentInfoSpikeDown);
}

void ARSpikes::StopSpike()
{
	GetWorld()->GetTimerManager().ClearTimer(ReverseTimerHandle);
	
	bSpikeResetComplete = true;
	constexpr EMoveComponentAction::Type MoveAction = EMoveComponentAction::Type::Stop;

	UKismetSystemLibrary::MoveComponentTo(
		SpikeMesh,
		FVector(0.f,0.f,-100.f),
		FRotator::ZeroRotator,
		true,
		true,
		SpikeDownDuration,
		false,
		MoveAction,
		LatentInfoSpikeDown);

	UKismetSystemLibrary::MoveComponentTo(
		SpikeMesh,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		true,
		true,
		SpikeUpDuration,
		false,
		MoveAction,
		LatentInfoSpikeUP);
}

void ARSpikes::OnMoveFinishedDown()
{
	bSpikeResetComplete = true;
}
