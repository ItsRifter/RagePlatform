// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlatformComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = ("Entity"), meta = (BlueprintSpawnableComponent))
class UPlatformComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UPlatformComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	bool MoveOnStart;

	UPROPERTY(EditDefaultsOnly)
	float MoveDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UInterpToMovementComponent* MoveComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class APlatformPath*> Paths;

	bool IsMoving;

	int NodeIndex;

	UFUNCTION(BlueprintCallable)
	void StartMoving();

	UFUNCTION(BlueprintCallable)
	void StopMoving();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
