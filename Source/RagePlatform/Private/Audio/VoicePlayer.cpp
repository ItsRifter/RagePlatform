// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/VoicePlayer.h"
#include <Framework/RGameInstance.h>
#include <Kismet/GameplayStatics.h>
#include "Player/RageCharacter.h"
#include "Traps/TrapEnum.h"

// Sets default values
AVoicePlayer::AVoicePlayer()
{

}

// Called when the game starts or when spawned
void AVoicePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	bCanPlay = true;

	GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance)
	{
		GameInstance->VoicelinePlayer = this;
	}
}

void AVoicePlayer::ReadyNextQuip()
{
	bWaitTimer = false;
	WaitHandle.Invalidate();
}

void AVoicePlayer::PlayQuip(ETrap QuipToPlay)
{
	if (!bCanPlay)
	{
		return;
	}

	int8 Chance = FMath::RandRange(1, 5);

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

	USoundBase* SoundToPlay = nullptr;

	switch (QuipToPlay)
	{
		case ETrap::Spikes:
			SoundToPlay = SpikeQuips[FMath::RandRange(0, SpikeQuips.Num()-1)];
			break;

		case ETrap::Axe:
			SoundToPlay = AxeQuips[FMath::RandRange(0, AxeQuips.Num()-1)];
			break;

		case ETrap::Chandelier:
			SoundToPlay = ChandelierQuips[FMath::RandRange(0, ChandelierQuips.Num()-1)];
			break;

		case ETrap::Explosion:
			SoundToPlay = ExplosionQuips[FMath::RandRange(0, ExplosionQuips.Num()-1)];
			break;

		case ETrap::Saw:
			SoundToPlay = SawQuips[FMath::RandRange(0, SawQuips.Num()-1)];
			break;

		case ETrap::LavaPit:
			SoundToPlay = LavaQuips[FMath::RandRange(0, LavaQuips.Num()-1)];
			break;

		case ETrap::PoisonPit:
			SoundToPlay = PoisonQuips[FMath::RandRange(0, PoisonQuips.Num()-1)];
			break;

		case ETrap::Arrows:
			SoundToPlay = ArrowQuips[FMath::RandRange(0, ArrowQuips.Num()-1)];
			break;
	}

	if (SoundToPlay)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundToPlay);
	}
}

bool AVoicePlayer::IsValidSoundArray(ETrap QuipCheck)
{
	switch (QuipCheck)
	{
		case ETrap::Spikes:       return SpikeQuips.Num() > 0;
		case ETrap::Axe:          return AxeQuips.Num() > 0;
		case ETrap::Chandelier:   return ChandelierQuips.Num() > 0;
		case ETrap::Explosion:    return ExplosionQuips.Num() > 0;
		case ETrap::Saw:          return SawQuips.Num() > 0;
		case ETrap::LavaPit:      return LavaQuips.Num() > 0;
		case ETrap::PoisonPit:    return PoisonQuips.Num() > 0;
		case ETrap::Arrows:		  return ArrowQuips.Num() > 0;

		default: return false;
	}
}
