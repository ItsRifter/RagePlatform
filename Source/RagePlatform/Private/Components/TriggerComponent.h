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

	UPROPERTY(EditInstanceOnly)
	bool bStartActive;

	UPROPERTY(EditInstanceOnly)
	EInteractHandle InteractType;

	UPROPERTY(EditInstanceOnly)
	float ReactivationTime;

	UPROPERTY(EditInstanceOnly)
	float HoldBeforeReset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* TriggerBox;

	UFUNCTION()
	void Trigger();

	UFUNCTION()
	void Reset();

	UFUNCTION(BlueprintImplementableEvent, Category = "Trap Logic")
	void OnReset();

	UFUNCTION()
	void DoReset();

	UFUNCTION()
	void TriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent, Category = "Trap Logic")
	void OnOverlap(ARageCharacter* player);

	bool bIsTrapReady;
	FTimerHandle TrapActiveHandle;
	FTimerHandle TrapResetHandle;
};
