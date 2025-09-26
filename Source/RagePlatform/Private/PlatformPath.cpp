// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformPath.h"

// Sets default values
APlatformPath::APlatformPath()
{
	PrimaryActorTick.bCanEverTick = false;

	NewDuration = 1.0f;
	TeleportToNext = false;
}

// Called when the game starts or when spawned
void APlatformPath::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlatformPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformPath::OnPass(class UPlatformComponent* platform)
{
	
}