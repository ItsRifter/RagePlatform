// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolNodes.generated.h"

UCLASS()
class APatrolNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere)
	float HoldPositionTime;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> NextNode;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> OptionalNode;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<AActor> BackNode;
};
