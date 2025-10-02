// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDeath ,bool, bIsDead);

/**
 * 
 */
UCLASS()
class URGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeath OnPlayerDeath;

	UFUNCTION(BlueprintCallable)
	void SetOnPlayerDeath(bool bIsDead);
};
