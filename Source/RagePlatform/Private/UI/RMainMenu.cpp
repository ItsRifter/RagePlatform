// Fill out your copyright notice in the Description page of Project Settings.


#include "RMainMenu.h"

#include "RLeaderboard.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "RPlayerName.h"
#include "Components/Image.h"
#include "Components/Overlay.h"

void URMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	//Main Menu Buttons
	PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	NewGameButton->OnClicked.AddDynamic(this, &URMainMenu::OnPlayButtonClicked);
	ContinueButton->OnClicked.AddDynamic(this, &URMainMenu::OnContinueButtonClicked);

	OptionsButton->OnClicked.AddDynamic(this, &URMainMenu::OnOptionsButtonClicked);

	LeaderboardButton->OnClicked.AddDynamic(this, &URMainMenu::OnLeaderboardButtonClicked);

	QuitButton->OnClicked.AddDynamic(this, &URMainMenu::OnQuitButtonClicked);

	//Continue Menu Buttons
	NewGameAcceptedButton->OnClicked.AddDynamic(this, &URMainMenu::OnPlayButtonClicked);
	ContinueBackButton->OnClicked.AddDynamic(this, &URMainMenu::OnBackButtonClicked);
}

void URMainMenu::OnPlayButtonClicked()
{
	if (PlayerNameBP != nullptr)
	{
		UUserWidget* TempWidget = CreateWidget(GetWorld(), PlayerNameBP);
		TempWidget->AddToViewport();
		PlayerNameWidget = Cast<URPlayerName>(TempWidget);
		RemoveFromParent();
	}
}

void URMainMenu::OnContinueButtonClicked()
{
	ContinueOverlay->SetVisibility(ESlateVisibility::Visible);
}

void URMainMenu::OnOptionsButtonClicked()
{
	if (OptionsMenuBP != nullptr)
	{
		UUserWidget* TempWidget = CreateWidget(GetWorld(), OptionsMenuBP);
		TempWidget->AddToViewport();
		//OptionsWidget = Cast<UROptionsMenu>(TempWidget);
		RemoveFromParent();
	}
}

void URMainMenu::OnLeaderboardButtonClicked()
{
	if (LeaderboardBP != nullptr)
	{
		UUserWidget* TempWidget = CreateWidget(this, LeaderboardBP);
		TempWidget->AddToViewport();
		LeaderboardWidget = Cast<URLeaderboard>(TempWidget);
		FSlateBrush SlateBrush;
		SlateBrush.TintColor = LeaderboardWidget->MainMenuColor;
		LeaderboardWidget->Background->SetBrush(SlateBrush);
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void URMainMenu::OnQuitButtonClicked()
{
	const TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit;
	UKismetSystemLibrary::QuitGame(this, PlayerController, QuitPreference, false);
}

void URMainMenu::OnBackButtonClicked()
{
	ContinueOverlay->SetVisibility(ESlateVisibility::Hidden);
}
