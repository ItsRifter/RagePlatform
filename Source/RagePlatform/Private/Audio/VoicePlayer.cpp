// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/VoicePlayer.h"
#include "TrapEnums.h"
#include <Framework/RGameInstance.h>
#include <Kismet/GameplayStatics.h>
#include "Player/RageCharacter.h"

// Sets default values
AVoicePlayer::AVoicePlayer()
{

}

// Called when the game starts or when spawned
void AVoicePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	URGameInstance* GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance)
	{
		GameInstance->VoicelinePlayer = this;
	}
}

void AVoicePlayer::ReadyNextQuip()
{
	bWaitTimer = false;
}

void AVoicePlayer::PlayQuip(ETrap QuipToPlay)
{
	int8 Chance = FMath::RandRange(1, 5);

	UE_LOGFMT(LogTemp, Warning, "Chance: {0}", Chance);

	if (Chance < 5 || bWaitTimer)
	{
		return;
	}

	bWaitTimer = true;

	GetWorld()->GetTimerManager().SetTimer(WaitHandle, this,
		&AVoicePlayer::ReadyNextQuip, 40.0f, false);

	if (!IsValidSoundArray(QuipToPlay))
	{
		UE_LOGFMT(LogAudio, Error, "Missing voiceline sounds for {0}", *UEnum::GetDisplayValueAsText(QuipToPlay).ToString());
		return;
	}

	switch (QuipToPlay)
	{
		case ETrap::Spikes:
			UGameplayStatics::PlaySound2D(GetWorld(), SpikeQuips[FMath::RandRange(0, SpikeQuips.Max() - 1)]);
			break;

		case ETrap::Axe:
			UGameplayStatics::PlaySound2D(GetWorld(), AxeQuips[FMath::RandRange(0, AxeQuips.Max() - 1)]);
			break;

		case ETrap::Chandelier:
			UGameplayStatics::PlaySound2D(GetWorld(), ChandelierQuips[FMath::RandRange(0, ChandelierQuips.Max() - 1)]);
			break;

		case ETrap::Explosion:
			UGameplayStatics::PlaySound2D(GetWorld(), ExplosionQuips[FMath::RandRange(0, ExplosionQuips.Max() - 1)]);
			break;

		case ETrap::Saw:
			UGameplayStatics::PlaySound2D(GetWorld(), SawQuips[FMath::RandRange(0, SawQuips.Max() - 1)]);
			break;

		case ETrap::LavaPit:
			UGameplayStatics::PlaySound2D(GetWorld(), LavaQuips[FMath::RandRange(0, LavaQuips.Max() - 1)]);
			break;

		case ETrap::PoisonPit:
			UGameplayStatics::PlaySound2D(GetWorld(), PoisonQuips[FMath::RandRange(0, PoisonQuips.Max() - 1)]);
			break;
	}
}

bool AVoicePlayer::IsValidSoundArray(ETrap QuipCheck)
{
	switch (QuipCheck)
	{
		case ETrap::Spikes:       return SpikeQuips.Num() != 0;
		case ETrap::Axe:          return AxeQuips.Num() != 0;
		case ETrap::Chandelier:   return ChandelierQuips.Num() != 0;
		case ETrap::Explosion:    return ExplosionQuips.Num() != 0;
		case ETrap::Saw:          return SawQuips.Num() != 0;
		case ETrap::LavaPit:      return LavaQuips.Num() != 0;
		case ETrap::PoisonPit:    return PoisonQuips.Num() != 0;

		default: return false;
	}
}
