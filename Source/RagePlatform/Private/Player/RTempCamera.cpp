// Fill out your copyright notice in the Description page of Project Settings.


#include "RTempCamera.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Traps/RSpikes.h"


// Sets default values
ARTempCamera::ARTempCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot"));

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>("PhysicsSphere");
	Sphere->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(Sphere);

	FocusOffset = FVector(0,0,0);
}

// Called when the game starts or when spawned
void ARTempCamera::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARTempCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpikeVar)
	{
		const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
			Camera->GetComponentLocation(),
			SpikeVar->GetActorLocation() + FVector(0.f, 0.f, 50.f));

		const FRotator FinalRotation = UKismetMathLibrary::RInterpTo(
			Camera->GetComponentRotation(),
			TargetRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			3.f);

		Camera->SetWorldRotation(FinalRotation);
	}

	if (FocusVar)
	{
		const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
			Camera->GetComponentLocation(),
			FocusVar->GetActorLocation() + (FocusOffset + FVector(0.f, 0.f, 50.f)));

		const FRotator FinalRotation = UKismetMathLibrary::RInterpTo(
			Camera->GetComponentRotation(),
			TargetRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			3.f);

		Camera->SetWorldRotation(FinalRotation);
	}
}

void ARTempCamera::StartFocus()
{
	SetActorTickEnabled(true);
}

void ARTempCamera::StopFocus()
{
	SetActorTickEnabled(false);
}
