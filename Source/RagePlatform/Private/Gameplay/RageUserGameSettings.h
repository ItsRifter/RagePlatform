// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "RageUserGameSettings.generated.h"

UCLASS()
class URageUserGameSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()
	
private:

	UPROPERTY(Config)
	float MasterVolume;

	UPROPERTY(Config)
	float SFXVolume;

	UPROPERTY(Config)
	float DialogueVolume;

	UPROPERTY(Config)
	float MusicVolume;

public:

	UFUNCTION(BlueprintCallable)
	static URageUserGameSettings* GetRageOptionGameUserSettings();

	UFUNCTION(BlueprintCallable, Category = "RageGame/Audio")
	void SetMasterVolume(float newVolume);

	UFUNCTION(BlueprintCallable, Category = "RageGame/Audio", meta = (DisplayName = "Set SFX Volume"))
	void SetSFXVolume(float newVolume);

	UFUNCTION(BlueprintCallable, Category = "RageGame/Audio")
	void SetDialogueVolume(float newVolume);

	UFUNCTION(BlueprintCallable, Category = "RageGame/Audio")
	void SetMusicVolume(float newVolume);

	UFUNCTION(BlueprintCallable, Category = "RageGame/Audio")
	float GetMasterVolume();

	UFUNCTION(BlueprintCallable, Category = "RageGame/Audio", meta = (DisplayName = "Get SFX Volume"))
	float GetSFXVolume();

	UFUNCTION(BlueprintCallable, Category = "RageGame/Audio")
	float GetDialogueVolume();

	UFUNCTION(BlueprintCallable, Category = "RageGame/Audio")
	float GetMusicVolume();
};
