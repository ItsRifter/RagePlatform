// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RMainMenu.generated.h"

class URPlayerName;
class URLeaderboard;
class UButton;

UCLASS()
class RAGEPLATFORM_API URMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PlayerNameBP;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LeaderboardBP;

	UPROPERTY()
	URPlayerName* PlayerNameWidget;

	UPROPERTY()
	URLeaderboard* LeaderboardWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* PlayButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* OptionsButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* LeaderboardButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly)
	FName GameLevelName;

	UFUNCTION()
	void OnPlayButtonClicked();

	UFUNCTION()
	void OnLeaderboardButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();
};