#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ROptionsMenu.generated.h"

class UButton;

UCLASS()
class UROptionsMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuBP;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* BackButton;

	UFUNCTION()
	void OnBackButtonClicked();
};
