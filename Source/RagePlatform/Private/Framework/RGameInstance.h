// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FString> LevelTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DeathCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString GameTime;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, const FText&, DeathText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameRestart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameEnd);

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

	UPROPERTY(BlueprintAssignable)
	FOnGameEnd OnGameEnd;

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

	UPROPERTY(BlueprintReadOnly)
	class AVoicePlayer* VoicelinePlayer;

	UPROPERTY(BlueprintReadWrite)
	FText PlayerName;

	UPROPERTY(BlueprintReadWrite)
	int32 LevelNumber;

	UPROPERTY(BlueprintReadWrite)
	FPlayerStats PlayerStats;

	UFUNCTION(BlueprintCallable)
	void SetOnPlayerDeath(const TArray<FText>& DeathText);
};
