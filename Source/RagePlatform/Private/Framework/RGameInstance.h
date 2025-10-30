// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, const FText&, DeathText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameRestart);

/**
 * 
 */
UCLASS()
class URGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable)
	FOnGameRestart OnGameRestart;

	UPROPERTY(BlueprintReadWrite)
	int32 DeathCount = 0;

	UPROPERTY(BlueprintReadWrite)
	bool bIsEasyMode = false;

	UPROPERTY(BlueprintReadWrite)
	bool bCanCountLevelTime;

	UPROPERTY(BlueprintReadWrite)
	bool bCanCountGameTime;

	UPROPERTY(BlueprintReadWrite)
	float TimeVar;

	UPROPERTY(BlueprintReadWrite)
	float GameTimeVar;

	UFUNCTION(BlueprintCallable)
	void SetOnPlayerDeath(const TArray<FText>& DeathText);
};
