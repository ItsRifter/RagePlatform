// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/UserWidget.h"
#include "RRestartWidget.generated.h"

class USizeBox;
class UTextBlock;
class ARageCharacter;
class URGameInstance;
class UButton;

UCLASS()
class URRestartWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* RestartButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* MenuButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY()
	APlayerController* PlayerController;
	
	UPROPERTY()
	ARageCharacter* PlayerCharacter;

	UPROPERTY()
	URGameInstance* GameInstance;

	UPROPERTY(EditDefaultsOnly)
	FName MainMenuLevel;

	UFUNCTION()
	void OnRestartButtonClicked();

	UFUNCTION()
	void OnMainMenuButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();

public:
	UPROPERTY()
	FTimerHandle VisibilityTimer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USizeBox* SizeBoxButtons;

	UPROPERTY(meta = (BindWidgetAnim), BlueprintReadWrite, Transient)
	UWidgetAnimation* RestartAnim;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* DeathText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* DeathsCountText;
};
