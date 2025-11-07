// Fill out your copyright notice in the Description page of Project Settings.


#include "RPlayerName.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"

void URPlayerName::NativeConstruct()
{
	Super::NativeConstruct();
	
	GameLevelName = TEXT("Level1");

	PlayerController = UGameplayStatics::GetPlayerController(this,0);
	GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(this));

	PlayerNameEditable->SetHintText(FText::FromString(TEXT("Your Name")));
	PlayerNameEditable->OnTextCommitted.AddDynamic(this, &URPlayerName::TextCommitted);
	
	BackButton->OnClicked.AddDynamic(this,&URPlayerName::OnBackButtonClicked);
	SubmitButton->OnClicked.AddDynamic(this, &URPlayerName::OnSubmitButtonClicked);
}

void URPlayerName::TextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (GameInstance)
	{
		GameInstance->PlayerName = Text;
	}
	
	if (CommitMethod == ETextCommit::OnEnter)
	{	
		OnSubmitButtonClicked();
	}
}

void URPlayerName::OnSubmitButtonClicked()
{
	if (GameLevelName != NAME_None && !GameInstance->PlayerName.IsEmpty())
	{
		UGameplayStatics::OpenLevel(this, GameLevelName, true);

		const FInputModeGameOnly InputModeGameOnly;
		PlayerController->SetInputMode(InputModeGameOnly);
		PlayerController->SetShowMouseCursor(false);
	}
}

void URPlayerName::OnBackButtonClicked()
{
	if (MainMenuBP != nullptr)
	{
		UUserWidget* TempWidget = CreateWidget(GetWorld(), MainMenuBP);
		TempWidget->AddToViewport();
		RemoveFromParent();
	}
}