// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/PatrolEnemy.h"

// Sets default values
APatrolEnemy::APatrolEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

}

// Called when the game starts or when spawned
void APatrolEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
//void APatrolEnemy::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
