// Fill out your copyright notice in the Description page of Project Settings.


#include "RGameEnd.h"

#include "RLeaderboard.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void URGameEnd::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	MainMenuButton->OnClicked.AddDynamic(this, &URGameEnd::OnMainMenuButtonClicked);
	LeaderboardButton->OnClicked.AddDynamic(this, &URGameEnd::OnLeaderboardButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &URGameEnd::OnQuitButtonClicked);
}

void URGameEnd::OnMainMenuButtonClicked()
{
	if (MainMenuLevelName != NAME_None)
	{
		UGameplayStatics::OpenLevel(this, MainMenuLevelName, true);
	}
}

void URGameEnd::OnLeaderboardButtonClicked()
{
	if (LeaderboardBP != nullptr)
	{
		UUserWidget* TempWidget = CreateWidget(this, LeaderboardBP);
		TempWidget->AddToViewport();
		LeaderboardWidget = Cast<URLeaderboard>(TempWidget);
		FSlateBrush SlateBrush;
		SlateBrush.TintColor = LeaderboardWidget->GameEndColor;
		LeaderboardWidget->Background->SetBrush(SlateBrush);
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void URGameEnd::OnQuitButtonClicked()
{
	const TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit;
	UKismetSystemLibrary::QuitGame(this, PlayerController, QuitPreference, false);
}
