// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformPath.h"

// Sets default values
APlatformPath::APlatformPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	MoveMultiplier = 1.0f;
}

// Called when the game starts or when spawned
void APlatformPath::BeginPlay()
{
	Super::BeginPlay();
	
}