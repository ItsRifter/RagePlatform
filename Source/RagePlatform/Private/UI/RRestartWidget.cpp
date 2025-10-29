// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Button.h"
#include "Framework/RGameInstance.h"
#include "UI/RRestartWidget.h"

#include "Components/SizeBox.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RageCharacter.h"

void URRestartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	PlayerController = UGameplayStatics::GetPlayerController(this,0);
	PlayerCharacter = Cast<ARageCharacter>(UGameplayStatics::GetPlayerCharacter(this,0));

	RestartButton->OnClicked.AddDynamic(this, &URRestartWidget::OnRestartButtonClicked);
	MenuButton->OnClicked.AddDynamic(this, &URRestartWidget::OnMainMenuButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &URRestartWidget::OnQuitButtonClicked);
	SizeBoxButtons->SetRenderOpacity(0.f);
	SizeBoxButtons->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void URRestartWidget::OnRestartButtonClicked()
{
	SizeBoxButtons->SetRenderOpacity(0.f);
	StopAnimation(RestartAnim);
	GetWorld()->GetTimerManager().ClearTimer(VisibilityTimer);
	SizeBoxButtons->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	if (GameInstance)
	{
		GameInstance->OnGameRestart.Broadcast();
		PlayerCharacter->bIsAlive = true;
	}
	if (PlayerController)
	{
		SetVisibility(ESlateVisibility::Hidden);
		if (PlayerCharacter)
		{
			PlayerCharacter->GetCharacterMovement()->MaxAcceleration = PlayerCharacter->SavedMaxAcceleration;
		}
		PlayerController->SetShowMouseCursor(false);
		const FInputModeGameOnly InputModeDataGame;
		PlayerController->SetInputMode(InputModeDataGame);
	}
}

void URRestartWidget::OnMainMenuButtonClicked()
{
	if (MainMenuLevel != NAME_None)
	{
		if (GameInstance)
		{
			GameInstance->DeathCount = 0;
			GameInstance->TimeVar = 0;
			GameInstance->bCanCountLevelTime = false;
			GameInstance->bCanCountGameTime = false;
			GameInstance->GameTimeVar = 0;
		}
		UGameplayStatics::OpenLevel(this,MainMenuLevel);
	}
}

void URRestartWidget::OnQuitButtonClicked()
{
	const TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit;
	UKismetSystemLibrary::QuitGame(this,PlayerController,QuitPreference,false);
}
