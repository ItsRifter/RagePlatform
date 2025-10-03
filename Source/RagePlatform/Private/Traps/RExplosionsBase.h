// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RExplosionsBase.generated.h"

class URGameInstance;
class USphereComponent;

UCLASS()
class RAGEPLATFORM_API ARExplosionsBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARExplosionsBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleDefaultsOnly)
	USphereComponent* DeathOverlap;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* ExplosionSound;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ExplosionParticleSystem;

	UPROPERTY()
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY()
	URGameInstance* GameInstance;

	UFUNCTION()
	void OnComponentBeginOverlapKillBox(UPrimitiveComponent* OverlappedComponent,
										AActor* OtherActor, UPrimitiveComponent* OtherComp,
										int32 OtherBodyIndex, bool bFromSweep,
										const FHitResult& SweepResult);

	UFUNCTION()
	void OnRestartDelegate();

	bool bExploded;
};
