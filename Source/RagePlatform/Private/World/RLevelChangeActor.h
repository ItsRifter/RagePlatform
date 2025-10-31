// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RLevelChangeActor.generated.h"

class URGameInstance;
class ARageCharacter;
class UBoxComponent;

UCLASS()
class ARLevelChangeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARLevelChangeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly)
	UBoxComponent* PlayerOverlap;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName NextLevel;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	ARageCharacter* PlayerCharacter;

	UPROPERTY()
	URGameInstance* GameInstance;

	UPROPERTY()
	float FadeDelay;

	FTimerHandle FadeHandle;
	bool bLevelChanged;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,
										  AActor* OtherActor, UPrimitiveComponent* OtherComp,
										  int32 OtherBodyIndex, bool bFromSweep,
										  const FHitResult& SweepResult);

	UFUNCTION()
	void OpenNextLevel();

	UFUNCTION(BlueprintImplementableEvent)
	void OnLevelStartChange();
};
