// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RLeaderboard.generated.h"

class UImage;
class UButton;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class RAGEPLATFORM_API URLeaderboard : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* PlayersVerticalBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuBP;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> EndGameBP;

	UFUNCTION()
	void OnBackButtonClicked();

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Background;
	
	UPROPERTY()
	FLinearColor MainMenuColor;

	UPROPERTY()
	FLinearColor GameEndColor;
};
