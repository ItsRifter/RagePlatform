// Fill out your copyright notice in the Description page of Project Settings.


#include "RLeaderboard.h"

#include "RMainMenu.h"
#include "Components/Button.h"
#include "Components/Image.h"

void URLeaderboard::NativeConstruct()
{
	Super::NativeConstruct();

	MainMenuColor = FLinearColor(0.f,0.2f,0.11f,0.25f);
	GameEndColor = FLinearColor(1.f,0.484f,0.f,0.31f);
	BackButton->OnClicked.AddDynamic(this,&URLeaderboard::OnBackButtonClicked);
	FSlateBrush SlateBrush;
	SlateBrush.TintColor = MainMenuColor;
	Background->SetBrush(SlateBrush);
}

void URLeaderboard::OnBackButtonClicked()
{
	if (UUserWidget* Parent = GetTypedOuter<UUserWidget>())
	{
		Parent->SetVisibility(ESlateVisibility::Visible);
		RemoveFromParent();
	}
}
