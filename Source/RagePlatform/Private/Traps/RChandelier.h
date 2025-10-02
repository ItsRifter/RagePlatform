// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RChandelier.generated.h"

class UBoxComponent;
class ARageCharacter;

UCLASS()
class ARChandelier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARChandelier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	ARageCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, meta = (UIMin = "1", UIMax = "10"))
	int32 FallSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* ChandelierMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* KillBox;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* PlayerOverlapBox;

	UPROPERTY(BlueprintReadWrite)
	FVector StartLocation;
	
	UPROPERTY(BlueprintReadWrite)
	FRotator StartRotation;

	UPROPERTY()
	URGameInstance* GameInstance;

	UFUNCTION()
	void OnDeathDelegate(bool bIsDead);

	UFUNCTION()
	void OnComponentBeginOverlapKillBox(UPrimitiveComponent* OverlappedComponent, 
								AActor* OtherActor, UPrimitiveComponent* OtherComp,
								int32 OtherBodyIndex, bool bFromSweep,
								const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnComponentBeginOverlapPlayerBox(UPrimitiveComponent* OverlappedComponent, 
								AActor* OtherActor, UPrimitiveComponent* OtherComp,
								int32 OtherBodyIndex, bool bFromSweep,
								const FHitResult& SweepResult);

	bool bChandelierFell;

};
