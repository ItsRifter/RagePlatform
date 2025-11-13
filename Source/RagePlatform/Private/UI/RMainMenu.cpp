// Fill out your copyright notice in the Description page of Project Settings.


#include "RMainMenu.h"

#include "RLeaderboard.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "RPlayerName.h"

void URMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayButton->OnClicked.AddDynamic(this, &URMainMenu::OnPlayButtonClicked);
	LeaderboardButton->OnClicked.AddDynamic(this, &URMainMenu::OnLeaderboardButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &URMainMenu::OnQuitButtonClicked);
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

void URMainMenu::OnLeaderboardButtonClicked()
{
	if (LeaderboardBP != nullptr)
	{
		UUserWidget* TempWidget = CreateWidget(GetWorld(), LeaderboardBP);
		TempWidget->AddToViewport();
		LeaderboardWidget = Cast<URLeaderboard>(TempWidget);
		RemoveFromParent();
	}
}

void URMainMenu::OnQuitButtonClicked()
{
	const TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit;
	UKismetSystemLibrary::QuitGame(this, PlayerController, QuitPreference, false);
}
