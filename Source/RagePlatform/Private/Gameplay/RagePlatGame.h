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
	UFUNCTION()
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	UFUNCTION()
	void RespawnPlayer();

	//Sets checkpoints for players to respawn to
	UFUNCTION()
	void SetCheckpoint(AActor* NewCheckpoint);

	//Resets the checkpoint
	UFUNCTION()
	void ResetCheckpoint();

protected:
	void CleanupDeadPawn(APawn* DeadPawn);

	UPROPERTY()
	AActor* CurCheckpoint;
};
