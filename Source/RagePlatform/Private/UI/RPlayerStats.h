// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPlayerStats.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class RAGEPLATFORM_API URPlayerStats : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* PlayerNameButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PlayerNameText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* DeathsCountText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* LvlOneTimeText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* LvlTwoTimeText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* LvlThreeTimeText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* LvlFourTimeText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* LvlFiveTimeText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* GameTime;

	UPROPERTY(meta = (BindWidgetAnim), BlueprintReadWrite, Transient)
	UWidgetAnimation* DropDown;

	bool bOpened;
	
public:
	UPROPERTY(BlueprintReadWrite)
	float GameTimeFloat;
	
	UFUNCTION(BlueprintCallable)
	void PlayDropDownAnimation();

	UFUNCTION(BlueprintCallable)
	void SetAllStats(
		FText PlayerName,
		FText Deaths,
		FText LvlOneTime,
		FText LvlTwoTime,
		FText LvlThreeTime,
		FText LvlFourTime,
		FText LvlFiveTime,
		FText GameTimeText,
		float InGameTimeFloat
	);
};
