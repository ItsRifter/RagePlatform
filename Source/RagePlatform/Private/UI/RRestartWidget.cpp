// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Button.h"
#include "Framework/RGameInstance.h"
#include "UI/RRestartWidget.h"

#include "Kismet/GameplayStatics.h"

void URRestartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	PlayerController = UGameplayStatics::GetPlayerController(this,0);

	RestartButton->OnClicked.AddDynamic(this, &URRestartWidget::OnButtonClicked);
}

void URRestartWidget::OnButtonClicked()
{
	if (GameInstance)
	{
		GameInstance->OnPlayerDeath.Broadcast(false);
	}
	if (PlayerController)
	{
		PlayerController->SetPause(false);
		PlayerController->SetShowMouseCursor(false);
		const FInputModeGameOnly InputModeDataGame;
		PlayerController->SetInputMode(InputModeDataGame);
		RemoveFromParent();
	}
}
