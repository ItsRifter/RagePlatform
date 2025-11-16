// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RGameEnd.generated.h"

class URLeaderboard;
class UButton;
/**
 * 
 */
UCLASS()
class RAGEPLATFORM_API URGameEnd : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* MainMenuButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* LeaderboardButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(EditDefaultsOnly)
	FName MainMenuLevelName;
	
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LeaderboardBP;

	UPROPERTY()
	URLeaderboard* LeaderboardWidget;

	UFUNCTION()
	void OnMainMenuButtonClicked();

	UFUNCTION()
	void OnLeaderboardButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();
};
