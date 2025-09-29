// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlatformComponent.generated.h"


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlatformComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlatformComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	bool MoveOnStart;

	UPROPERTY(EditDefaultsOnly)
	float MoveDuration;

	UPROPERTY(VisibleDefaultsOnly)
	class UInterpToMovementComponent* MoveComponent;

	UFUNCTION(BlueprintCallable)
	void StartMoving();

	UFUNCTION(BlueprintCallable)
	void StopMoving();

	//bool IsMoving;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
