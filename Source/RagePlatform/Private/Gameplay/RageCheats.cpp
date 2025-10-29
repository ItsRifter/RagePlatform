// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/RageCheats.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"

void URageCheats::ToggleDifficulty()
{
	if (URGameInstance* GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GameInstance->bIsEasyMode = !GameInstance->bIsEasyMode;

		UE_LOGFMT(LogTemp, Display, "Toggled easy difficulty to {0}", GameInstance->bIsEasyMode ? TEXT("True") : TEXT("False"));
	}
}