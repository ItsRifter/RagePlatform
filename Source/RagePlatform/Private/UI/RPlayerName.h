#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPlayerName.generated.h"

class UButton;
class URGameInstance;
class UEditableText;

UCLASS()
class RAGEPLATFORM_API URPlayerName : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly)
	FName GameLevelName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UEditableText* PlayerNameEditable;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* SubmitButton;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuBP;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	URGameInstance* GameInstance;

	UFUNCTION()
	void TextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnBackButtonClicked();

	UFUNCTION()
	void OnSubmitButtonClicked();
};