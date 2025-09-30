// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KillComponent.generated.h"

class UPrimitiveComponent;
class AActor;

struct FHitResult;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKillOverlap);

UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UKillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKillComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	bool StartReady;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBoxComponent* KillBox;

	/*UPROPERTY(BlueprintAssignable)
	FOnKillOverlap OnKillOverlap();*/

	UFUNCTION(BlueprintCallable)
	void SetEnabled();

	UFUNCTION(BlueprintCallable)
	void SetDisabled();

	UFUNCTION()
	void KillBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
