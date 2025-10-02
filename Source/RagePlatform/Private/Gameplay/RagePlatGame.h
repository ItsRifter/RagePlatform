// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RagePlatGame.generated.h"

UCLASS()
class ARagePlatGame : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void RespawnPlayer(AController* PlayerController);

protected:
	void CleanupDeadPawn(APawn* DeadPawn);
};
