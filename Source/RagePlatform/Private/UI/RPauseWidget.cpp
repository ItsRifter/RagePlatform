#include "UI/RPauseWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"

void URPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ResumeButton->OnClicked.AddDynamic(this, &URPauseWidget::OnResumeButtonClicked);
	MainMenuButton->OnClicked.AddDynamic(this, &URPauseWidget::OnMainMenuButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &URPauseWidget::OnQuitButtonClicked);
}

void URPauseWidget::OnResumeButtonClicked()
{
	UGameplayStatics::SetGamePaused(this, false);
	const FInputModeGameOnly InputModeGameOnly;
	PlayerController->SetInputMode(InputModeGameOnly);
	PlayerController->SetShowMouseCursor(false);
	SetVisibility(ESlateVisibility::Hidden);
}

void URPauseWidget::OnMainMenuButtonClicked()
{
	if (MainMenuLevelName != NAME_None)
	{
		if (GameInstance)
		{
			GameInstance->OnGameClosed.Broadcast();
			GameInstance->DeathCount = 0;
			GameInstance->TimeVar = 0;
			GameInstance->bCanCountLevelTime = false;
			GameInstance->GameTimeVar = 0;
			GameInstance->bCanCountGameTime = false;
		}
		UGameplayStatics::OpenLevel(this, MainMenuLevelName, true);
	}
}

void URPauseWidget::OnQuitButtonClicked()
{
	const TEnumAsByte<EQuitPreference::Type> QuitPreference = EQuitPreference::Quit;
	UKismetSystemLibrary::QuitGame(this, PlayerController, QuitPreference, false);
}