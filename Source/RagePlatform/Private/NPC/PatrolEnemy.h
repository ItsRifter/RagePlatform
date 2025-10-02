// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PatrolEnemy.generated.h"

UCLASS()
class APatrolEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APatrolEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	AActor* LastNode;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
