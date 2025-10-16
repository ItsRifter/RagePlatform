// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RTempCamera.generated.h"

class UCameraComponent;
class ARSpikes;

UCLASS()
class RAGEPLATFORM_API ARTempCamera : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARTempCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly)
	UStaticMeshComponent* Sphere;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly)
	UCameraComponent* Camera;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	ARSpikes* SpikeVar;

	UPROPERTY(BlueprintReadWrite)
	class AActor* FocusVar;

	UPROPERTY()
	FVector FocusOffset;

	UFUNCTION(BlueprintCallable)
	void StartFocus();
	
	UFUNCTION(BlueprintCallable)
	void StopFocus();
};
