
#include "UI/ROptionsMenu.h"
#include "Components/Button.h"

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