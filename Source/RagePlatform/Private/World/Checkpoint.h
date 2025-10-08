// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

UCLASS()
class ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

private:
	static int32 NextCheckpointIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

//#if WITH_EDITOR
//	virtual void OnConstruction(const FTransform& Transform) override;
//#endif

	/*UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* TriggerBox;

	UPROPERTY(EditDefaultsOnly)
	int32 CheckpointIndex;*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
