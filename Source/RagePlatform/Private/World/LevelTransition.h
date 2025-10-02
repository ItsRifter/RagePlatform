// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransition.generated.h"

class UPrimitiveComponent;
class AActor;
class UBoxComponent;

UCLASS()
class ALevelTransition : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelTransition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* DefaultSceneRoot;

	//The level name to transition to
	UPROPERTY(EditInstanceOnly)
	FString NextLevelName;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* TriggerBox;

	UFUNCTION()
	void OnLevelTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

public:	

};
