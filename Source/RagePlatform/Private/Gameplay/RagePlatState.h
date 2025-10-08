// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RagePlatState.generated.h"

/**
 * 
 */
UCLASS()
class ARagePlatState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void DeleteActorByTag(FName Tag);

};
