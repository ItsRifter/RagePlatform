// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformPath.generated.h"

UCLASS()
class APlatformPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlatformPath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	float NewDuration;

	UPROPERTY(EditDefaultsOnly)
	bool TeleportToNext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveMultiplier;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnPass();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
