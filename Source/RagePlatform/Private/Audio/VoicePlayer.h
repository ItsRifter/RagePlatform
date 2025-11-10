// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VoicePlayer.generated.h"

class USoundBase;

UCLASS(Blueprintable)
class AVoicePlayer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVoicePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class URGameInstance* GameInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Quips")
	TArray<USoundBase*> SpikeQuips;

	UPROPERTY(EditDefaultsOnly, Category = "Quips")
	TArray<USoundBase*> AxeQuips;

	UPROPERTY(EditDefaultsOnly, Category = "Quips")
	TArray<USoundBase*> ChandelierQuips;

	UPROPERTY(EditDefaultsOnly, Category = "Quips")
	TArray<USoundBase*> LavaQuips;

	UPROPERTY(EditDefaultsOnly, Category = "Quips")
	TArray<USoundBase*> PoisonQuips;

	UPROPERTY(EditDefaultsOnly, Category = "Quips")
	TArray<USoundBase*> SawQuips;

	UPROPERTY(EditDefaultsOnly, Category = "Quips")
	TArray<USoundBase*> ExplosionQuips;

	UPROPERTY(EditDefaultsOnly, Category = "Quips")
	TArray<USoundBase*> ArrowQuips;

	UPROPERTY()
	bool bWaitTimer;

	UPROPERTY()
	FTimerHandle WaitHandle;

	UFUNCTION()
	void ReadyNextQuip();

public:

	UPROPERTY(BlueprintReadWrite)
	bool bCanPlay;

	UFUNCTION(BlueprintCallable)
	void PlayQuip(ETrap QuipToPlay);

private:

	bool IsValidSoundArray(ETrap QuipCheck);
};