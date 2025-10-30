// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RMainMenu.generated.h"

class UButton;

UCLASS()
class RAGEPLATFORM_API URMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* PlayButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* OptionsButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UCheckBox* EasyToggleBox;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly)
	FName GameLevelName;

	UFUNCTION()
	void OnPlayButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();

	UFUNCTION()
	void OnEasyToggle(bool bIsChecked);
};
