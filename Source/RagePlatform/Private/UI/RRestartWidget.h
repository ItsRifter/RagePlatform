// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RRestartWidget.generated.h"

class URGameInstance;
class UButton;
/**
 * 
 */
UCLASS()
class URRestartWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* RestartButton;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	URGameInstance* GameInstance;

	UFUNCTION()
	void OnButtonClicked();
};
