// Fill out your copyright notice in the Description page of Project Settings.


#include "Traps/RDeathPit.h"

#include "Audio/VoicePlayer.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/RageCharacter.h"

// Sets default values
ARDeathPit::ARDeathPit()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot"));

	DeathPitMesh = CreateDefaultSubobject<UStaticMeshComponent>("DeathPit");
	DeathPitMesh->SetupAttachment(GetRootComponent());

	DeathTrigger = CreateDefaultSubobject<UBoxComponent>("DeathTrigger");
	DeathTrigger->SetupAttachment(GetRootComponent());

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
	AudioComponent->SetupAttachment(GetRootComponent());

	LoopSound = nullptr;

	PitType = EPitType::Unspecified;
	KillTexts.Add(FText::FromString(TEXT("You Tried to Swim in Lava!!")));
}

// Called when the game starts or when spawned
void ARDeathPit::BeginPlay()
{
	Super::BeginPlay();

	if (LoopSound)
	{
		AudioComponent->SetSound(LoopSound);
		AudioComponent->Play();
	}

	GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(this));

	DeathTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARDeathPit::OnComponentBeginOverlapKillBox);
}

void ARDeathPit::OnComponentBeginOverlapKillBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                const FHitResult& SweepResult)
{
	if (ARageCharacter* PlayerCharacter = Cast<ARageCharacter>(OtherActor))
	{
		if (!PlayerCharacter->bIsAlive)
		{
			return;
		}

		const int32 Index = UKismetMathLibrary::RandomIntegerInRange(0,KillTexts.Num() - 1);
		
		GameInstance->OnDeath.Broadcast(KillTexts[Index]);
		
		if (GameInstance->VoicelinePlayer)
		{
			switch (PitType)
			{
				case EPitType::Lava: GameInstance->VoicelinePlayer->PlayQuip(EQuip::LavaPit);
				case EPitType::Poison: GameInstance->VoicelinePlayer->PlayQuip(EQuip::PoisonPit);
			}
		}

		if (KillSound)
		{
			AudioComponent->SetSound(KillSound);
			AudioComponent->Play();
		}
		
		PlayerCharacter->DrownPlayer(
			PlayerCharacter->GetActorLocation() - FVector(0.f, 0.f, 60.f) +
			PlayerCharacter->GetActorForwardVector() * 20.f);

		OnKilledPlayer(PlayerCharacter);
	}
}
