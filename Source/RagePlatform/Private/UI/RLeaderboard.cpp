// Fill out your copyright notice in the Description page of Project Settings.


#include "RLeaderboard.h"

#include "Components/Button.h"

void URLeaderboard::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnClicked.AddDynamic(this,&URLeaderboard::OnBackButtonClicked);
}

void URLeaderboard::OnBackButtonClicked()
{
	if (MainMenuBP != nullptr)
	{
		UUserWidget* TempWidget = CreateWidget(GetWorld(), MainMenuBP);
		TempWidget->AddToViewport();
		RemoveFromParent();
	}
}
