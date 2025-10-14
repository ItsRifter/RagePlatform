// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RSpikes.generated.h"

class URGameInstance;
class ARageCharacter;
class UBoxComponent;

UCLASS()
class RAGEPLATFORM_API ARSpikes : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARSpikes();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	FLatentActionInfo LatentInfoSpikeUP;
	FLatentActionInfo LatentInfoSpikeDown;
	FTimerHandle ReverseTimerHandle;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* SpikeMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UBoxComponent* OverlapBox;

	UPROPERTY()
	ARageCharacter* PlayerCharacter;

	UPROPERTY()
	URGameInstance* GameInstance;

	UPROPERTY()
	float SpikeUpDuration;

	UPROPERTY()
	float SpikeDownDuration;

	UPROPERTY()
	bool bSpikeResetComplete;

	UPROPERTY(EditAnywhere)
	FVector PlayerImpact;

	UPROPERTY(EditDefaultsOnly, Category = "DeathScreen")
	TArray<FText> KillTexts;

	UFUNCTION()
	void OnRestartDelegate();

	UFUNCTION()
	void OnMoveFinished();

	UFUNCTION()
	void OnMoveFinishedDown();

	UFUNCTION()
	void ReverseSpike();

	UFUNCTION()
	void StopSpike();

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	                             AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                             int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);
};
