// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerStats.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void URPlayerStats::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerNameButton->OnClicked.AddDynamic(this, &URPlayerStats::PlayDropDownAnimation);
}

void URPlayerStats::PlayDropDownAnimation()
{
	if (!IsAnimationPlaying(DropDown))
	{
		if (bOpened)
		{
			PlayAnimation(
				DropDown,
				0,
				1,
				EUMGSequencePlayMode::Reverse
				,1,
				false);

			bOpened = false;
		}
		else
		{
			PlayAnimation(
				DropDown,
				0,
				1,
				EUMGSequencePlayMode::Forward
				,1,
				false);

			bOpened = true;
		}
	}
}

void URPlayerStats::SetAllStats(const FText PlayerName, const FText Deaths, const FText LvlOneTime, const FText LvlTwoTime, const FText LvlThreeTime,
	const FText LvlFourTime, const FText LvlFiveTime, const FText GameTimeText)
{
	PlayerNameText->SetText(PlayerName);
	DeathsCountText->SetText(Deaths);
	LvlOneTimeText->SetText(LvlOneTime);
	LvlTwoTimeText->SetText(LvlTwoTime);
	LvlThreeTimeText->SetText(LvlThreeTime);
	LvlFourTimeText->SetText(LvlFourTime);
	LvlFiveTimeText->SetText(LvlFiveTime);
	GameTime->SetText(GameTimeText);
}
