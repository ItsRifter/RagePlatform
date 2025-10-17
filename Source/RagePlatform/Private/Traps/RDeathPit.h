// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RDeathPit.generated.h"

class URGameInstance;
class UBoxComponent;
class USoundBase;

UCLASS()
class ARDeathPit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARDeathPit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* DeathPitMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* DeathTrigger;

	UPROPERTY(EditAnywhere, Category = "DeathScreen")
	TArray<FText> KillTexts;

	UPROPERTY()
	URGameInstance* GameInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* LoopSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* KillSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPoison;

	UFUNCTION()
	void OnComponentBeginOverlapKillBox(UPrimitiveComponent* OverlappedComponent,
										AActor* OtherActor, UPrimitiveComponent* OtherComp,
										int32 OtherBodyIndex, bool bFromSweep,
										const FHitResult& SweepResult);
	
};
