// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h" 
#include "Player/RageCharacter.h"
#include "Player/RTempCamera.h"
#include "Traps/SawTrap.h"

ASawTrap::ASawTrap()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	SkeletalMesh->SetupAttachment(GetRootComponent());
	
	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("SawTimeline"));

	KillText = FText::FromString(TEXT("You got cut up by a Sawblade!"));

	//ImpactVelocity = FVector(0.0f, 0.0f, 50.0f);
	MoveDistance = 45.0f;
}

void ASawTrap::BeginPlay()
{
	Super::BeginPlay();

	bIsReversed = false;
	StartLocation = DefaultSceneRoot->GetRelativeLocation();

	FOnTimelineFloat TimelineProgressFunc;

	FOnTimelineEvent TimelineFinish;

	UCurveFloat* Curve = NewObject<UCurveFloat>(this);

	Curve->FloatCurve.UpdateOrAddKey(0.0f, 0.0f);
	Curve->FloatCurve.UpdateOrAddKey(1.0f, 1.0f);

	TimelineProgressFunc.BindUFunction(this, TEXT("OnTimelineProgress"));
	TimelineFinish.BindUFunction(this, TEXT("OnTimelineFinish"));

	TimelineComponent->SetTimelineFinishedFunc(TimelineFinish);

	TimelineComponent->AddInterpFloat(Curve, TimelineProgressFunc);
	TimelineComponent->SetTimelineLengthMode(TL_LastKeyFrame);

	TimelineComponent->PlayFromStart();

	if (TriggerBox)
	{
		TriggerBox->Deactivate();
	}
}


void ASawTrap::OnTimelineFinish()
{
	bIsReversed = !bIsReversed;

	if (bIsReversed)
	{
		OnReverseDirection();
		TimelineComponent->Reverse();
	}
	else 
	{
		OnForwardDirection();
		TimelineComponent->Play();
	}
}

void ASawTrap::OnTimelineProgress(float val)
{
	float CurrentLocation = FMath::Sin(val * 1.0) * MoveDistance * 2;

	FVector TargetLocation = StartLocation + 
		DefaultSceneRoot->GetForwardVector() * CurrentLocation;

	DefaultSceneRoot->SetRelativeLocation(TargetLocation);
}

void ASawTrap::OnDeathDelegate(const FText& DeathText)
{
}

void ASawTrap::OnRestartDelegate()
{
}

void ASawTrap::KillPlayer(ARageCharacter* Player)
{
	Super::KillPlayer(Player);

	FVector LeftVector = -DefaultSceneRoot->GetRightVector() * 50.0f;

	ImpactVelocity = FVector(0.0f, 0.0f, 40.0f) + LeftVector;

	Player->PlayerFall(ImpactVelocity);

	if (Player->Temp_Camera)
	{
		Player->Temp_Camera->FocusVar = this;
		Player->Temp_Camera->FocusOffset = DeathFocusOffset;
		Player->Temp_Camera->StartFocus();
	}
}
