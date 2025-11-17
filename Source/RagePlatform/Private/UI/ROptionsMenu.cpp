
#include "Components/Button.h"
#include "UI/ROptionsMenu.h"

void UROptionsMenu::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnClicked.AddDynamic(this, &UROptionsMenu::OnBackButtonClicked);
}

void UROptionsMenu::OnBackButtonClicked()
{
	if (MainMenuBP != nullptr)
	{
		UUserWidget* TempWidget = CreateWidget(GetWorld(), MainMenuBP);
		TempWidget->AddToViewport();
		RemoveFromParent();
	}
}