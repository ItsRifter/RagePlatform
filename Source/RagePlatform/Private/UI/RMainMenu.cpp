// Fill out your copyright notice in the Description page of Project Settings.


#include "RMainMenu.h"

#include "RPlayerName.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"

void URMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = UGameplayStatics::GetPlayerController(this,0);
	PlayButton->OnClicked.AddDynamic(this, &URMainMenu::OnPlayButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &URMainMenu::OnQuitButtonClicked);

	EasyToggleBox->OnCheckStateChanged.AddDynamic(this, &URMainMenu::OnEasyToggle);
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

void URMainMenu::OnQuitButtonClicked()
{
	const TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit;
	UKismetSystemLibrary::QuitGame(this,PlayerController,QuitPreference,false);
}

void URMainMenu::OnEasyToggle(bool bIsChecked)
{
	URGameInstance* GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(this));

	if (GameInstance)
	{
		GameInstance->bIsEasyMode = bIsChecked;
	}
}
