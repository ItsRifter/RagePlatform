// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPauseWidget.generated.h"

class URGameInstance;
class UButton;
class UTextBlock;

UCLASS()
class URPauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* MainMenuButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY()
	URGameInstance* GameInstance;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly)
	FName MainMenuLevelName;

	UFUNCTION()
	void OnResumeButtonClicked();

	UFUNCTION()
	void OnMainMenuButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TimeText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* GameTimeText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PauseText;
};
