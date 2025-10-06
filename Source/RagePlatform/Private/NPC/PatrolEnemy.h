// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PatrolEnemy.generated.h"

class APatrolNode;

UCLASS()
class APatrolEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APatrolEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* PathChecker;

	UPROPERTY(VisibleDefaultsOnly)
	class UInterpToMovementComponent* MoveComponent;

	/*The starting node for this patrol enemy*/
	UPROPERTY(EditInstanceOnly, Category = "Patrol Enemy")
	TObjectPtr<AActor> StartNode;

	/*The speed multiplier for this patrol enemy*/
	UPROPERTY(EditInstanceOnly, Category = "Patrol Enemy", meta = (ClampMin = 0.0f, ClampMax = 3.0f))
	float MoveSpeed;

	/*The time it will take for this enemy to move to a node*/
	UPROPERTY(EditInstanceOnly, Category = "Patrol Enemy", meta = (ClampMin = 0.0f, ClampMax = 10.0f))
	float MoveDuration;

	UFUNCTION()
	void OnNodePass(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:
	//The target node to reach
	UPROPERTY()
	TObjectPtr<AActor> TargetNode;

	//The last node passed before reaching the target
	UPROPERTY()
	TObjectPtr<AActor> LastNode;

	FTimerHandle WaitHandle;

	/*Check if pathing doesn't go back and forth, returns true if should proceed forward*/
	UFUNCTION()
	bool CheckPathing(AActor* Goto, AActor* From);

	UFUNCTION()
	void MoveToNext();

public:	

};
