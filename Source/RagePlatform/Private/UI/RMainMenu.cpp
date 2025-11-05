// Fill out your copyright notice in the Description page of Project Settings.


#include "RMainMenu.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void URMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = UGameplayStatics::GetPlayerController(this,0);
	PlayButton->OnClicked.AddDynamic(this, &URMainMenu::OnPlayButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &URMainMenu::OnQuitButtonClicked);
}

void URMainMenu::OnPlayButtonClicked()
{
	if (GameLevelName != NAME_None)
	{
		UGameplayStatics::OpenLevel(this,GameLevelName,true);

		const FInputModeGameOnly InputModeGameOnly;
		PlayerController->SetInputMode(InputModeGameOnly);
		PlayerController->SetShowMouseCursor(false);
	}
}

void URMainMenu::OnQuitButtonClicked()
{
	const TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit;
	UKismetSystemLibrary::QuitGame(this,PlayerController,QuitPreference,false);
}
