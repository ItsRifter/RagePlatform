// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TriggerComponent.generated.h"


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTriggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	bool StartActive;

	UPROPERTY(EditAnywhere)
	bool TrapStartReady;

	UPROPERTY(EditAnywhere)
	float ReactivationTime;

	UPROPERTY(EditAnywhere)
	float HoldBeforeReset;

	UFUNCTION()
	void StartTrap();

	UFUNCTION()
	void TrapReset();

	UFUNCTION(BlueprintImplementableEvent, Category = "Trap Logic")
	void OnTrapReset();

	UFUNCTION()
	void TrapRetract();

	UFUNCTION()
	void TrapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, Category = "Trap Logic")
	void OnTrapOverlap(ARageCharacter* player);

	bool IsTrapReady;
	FTimerHandle TrapActiveHandle;
	FTimerHandle TrapResetHandle;
};
