// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/RGameInstance.h"
#include "Kismet/KismetMathLibrary.h"

void URGameInstance::SetOnPlayerDeath(const TArray<FText>& DeathTexts)
{
	const int32 Index = UKismetMathLibrary::RandomIntegerInRange(0,DeathTexts.Num() - 1);
	OnDeath.Broadcast(DeathTexts[Index]);
}

void URGameInstance::SetOnGameEnd()
{
	OnGameEnd.Broadcast();
}


