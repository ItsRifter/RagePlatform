// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/RGameInstance.h"
#include "Gameplay/RagePlatGame.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStatics.h"
#include "World/Checkpoint.h"

#include "RagePlatGame.h"

void ARagePlatGame::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	URGameInstance* GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance)
	{
		GameInstance->OnGameRestart.AddDynamic(this, &ARagePlatGame::RespawnPlayer);
	}

	CurCheckpoint = nullptr;
}

void ARagePlatGame::RespawnPlayer()
{
	AController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	AActor* Spawn = FindPlayerStart(PlayerController);

	if (CurCheckpoint != nullptr)
	{
		Spawn = CurCheckpoint;
	}

	if (Spawn)
	{
		FVector Location = Spawn->GetActorLocation();
		FRotator Rotation = Spawn->GetActorRotation();

		if (CurCheckpoint != nullptr)
		{
			ACheckpoint* Checkpoint = Cast<ACheckpoint>(CurCheckpoint);
			if (Checkpoint)
			{
				Rotation = Checkpoint->GetSpawnRotation();
			}
		}

		PlayerController->GetPawn()->SetActorLocation(Location);
		PlayerController->SetControlRotation(Rotation);
	}
	//No player spawns
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("No player spawnpoints found, add one!"))
	}
}

void ARagePlatGame::SetCheckpoint(AActor* NewCheckpoint)
{
	//Checkpoint is already set to this
	if (CurCheckpoint && CurCheckpoint == NewCheckpoint)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Checkpoint Set"));
	CurCheckpoint = NewCheckpoint;
}

void ARagePlatGame::ResetCheckpoint()
{
	//No checkpoint exists
	if (!CurCheckpoint)
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Checkpoint Reset"));
	CurCheckpoint = nullptr;
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