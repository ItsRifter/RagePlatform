// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

UENUM(BlueprintType)
enum class EInteractHandle : uint8
{
	DeleteActor				UMETA(DisplayName = "Delete"),
	SetActorVisibility		UMETA(DisplayName = "Set Visiblity"),
	ToggleComponent			UMETA(DisplayName = "Toggle Component")
};

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly)
	EInteractHandle InteractType;

	UPROPERTY(EditInstanceOnly)
	FName TargetFindTag;

	/*Depending on 'InteractType' will toggle behaviours of that actor,
	This won't work with specific 'InteractType' values*/
	UPROPERTY(EditInstanceOnly)
	bool Toggle;

	/*Delete this actor after using*/
	UPROPERTY(EditInstanceOnly)
	bool DeleteOnUse;

	UFUNCTION(BlueprintCallable)
	void Use();

private:
	UPROPERTY()
	TArray<AActor*> FoundActors;

public:	
		
};
