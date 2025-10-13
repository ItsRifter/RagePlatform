// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RGameInstance.h"

void URGameInstance::SetOnPlayerDeath(const FText& DeathText)
{
	OnDeath.Broadcast(DeathText);
}
