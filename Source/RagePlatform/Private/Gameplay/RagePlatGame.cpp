// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/RagePlatGame.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "RagePlatGame.h"

void ARagePlatGame::RespawnPlayer(AController* PlayerController)
{
	if (!PlayerController)
	{
		return;
	}

	APawn* DeadPawn = PlayerController->GetPawn();

	if (DeadPawn)
	{
		CleanupDeadPawn(DeadPawn);
	}

	AActor* Spawn = FindPlayerStart(PlayerController);

	if (Spawn)
	{
		FVector Location = Spawn->GetActorLocation();
		FRotator Rotation = Spawn->GetActorRotation();

		UClass* PawnClass = GetDefaultPawnClassForController(PlayerController);
		
		if (!PawnClass)
		{
			return;
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		APawn* NewPawn = GetWorld()->SpawnActor<APawn>(PawnClass, Location, Rotation, SpawnParams);

		if (NewPawn)
		{
			PlayerController->Possess(NewPawn);
		}
		//Failed to spawn the player at spawnpoint
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn new pawn for player controller: %s"), *PlayerController->GetName());
		}
	}
	//No player spawns
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("No player spawnpoints found, add one!"))
	}
}

//Cleanup the last pawn the player had
void ARagePlatGame::CleanupDeadPawn(APawn* DeadPawn)
{
	if (DeadPawn->GetController())
	{
		DeadPawn->GetController()->UnPossess();
	}

	DeadPawn->Destroy();
}