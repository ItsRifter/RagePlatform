// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolNodes.generated.h"

UCLASS()
class APatrolNodes : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolNodes();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float HoldPositionTime;

	UPROPERTY(EditAnywhere)
	AActor* NextNode;

	UPROPERTY(EditAnywhere)
	AActor* OptionalNode;

public:	

};
