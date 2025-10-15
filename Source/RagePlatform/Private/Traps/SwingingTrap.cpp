#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h" 
#include "Player/RageCharacter.h"
#include "Player/RTempCamera.h"
#include "Traps/SwingingTrap.h"

ASwingingTrap::ASwingingTrap()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	SkeletalMesh->SetupAttachment(GetRootComponent());

	KillBox->SetupAttachment(SkeletalMesh, SocketName);

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("SwingTimeline"));

	KillText = FText::FromString(TEXT("You got chopped by a Swinging Axe!"));

	SwingSpeed = 1.5f;
	SwingAngle = 180.0f;
}

void ASwingingTrap::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	KillBox->SetupAttachment(SkeletalMesh, SocketName);
}

void ASwingingTrap::BeginPlay()
{
	Super::BeginPlay();

	bDoPlayerKill = true;
	bGoReverse = false;

	StartRotation = DefaultSceneRoot->GetRelativeRotation();

	FOnTimelineFloat TimelineProgressFunc;

	UCurveFloat* Curve = NewObject<UCurveFloat>(this);

	Curve->FloatCurve.UpdateOrAddKey(0.0f, 0.0f);
	Curve->FloatCurve.UpdateOrAddKey(1.0f, 1.0f);
	Curve->FloatCurve.UpdateOrAddKey(2.05f, -0.05f);
	Curve->FloatCurve.UpdateOrAddKey(2.15f, 0.0f);

	TimelineProgressFunc.BindUFunction(this, TEXT("OnTimelineProgress"));

	TimelineComponent->AddInterpFloat(Curve, TimelineProgressFunc);
	TimelineComponent->SetTimelineLengthMode(TL_LastKeyFrame);

	TimelineComponent->SetLooping(true);
	TimelineComponent->PlayFromStart();

	if (TriggerBox)
	{
		TriggerBox->Deactivate();
	}

	if (SocketName != NAME_None)
	{
		KillBox->AttachToComponent(SkeletalMesh, 
			FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}
}

void ASwingingTrap::OnTimelineProgress(float val)
{
	float CurrentAngle = FMath::Sin(val * SwingSpeed) * SwingAngle;
	
	if (LastVal > val)
	{
		bSwingingLeft = true;
	}
	else if(LastVal < val)
	{
		bSwingingLeft = false;
	}

	LastVal = val;

	FRotator SwingRotation(0.0f, 0.0f, CurrentAngle);

	SwingRotation += StartRotation;

	DefaultSceneRoot->SetRelativeRotation(SwingRotation);
}

void ASwingingTrap::OnRestartDelegate()
{
}

void ASwingingTrap::OnDeathDelegate(const FText& DeathText)
{
}

void ASwingingTrap::KillPlayer(ARageCharacter* Player)
{
	Super::KillPlayer(Player);

	float CurRoll = FMath::Abs(GetRootComponent()->GetRelativeRotation().Roll);

	float Velocity = 0.0f;

	//Set velocity based on where this is swinging
	if (bSwingingLeft)
	{
		Velocity = 250.0f;
	}
	else
	{
		Velocity = -250.0f;
	}

	//Check to apply velocity in the correct direction
	if ((CurRoll >= 0 && CurRoll <= 90) || (CurRoll >= 180 && CurRoll <= 270))
	{
		ImpactVelocity = FVector(-Velocity, 0, 75.0f);
	}
	else 
	{
		ImpactVelocity = FVector(0, Velocity, 75.0f);
	}

	Player->PlayerFall(ImpactVelocity);

	if (Player->Temp_Camera)
	{
		Player->Temp_Camera->FocusVar = this;
		Player->Temp_Camera->FocusOffset = DeathFocusOffset;
		Player->Temp_Camera->StartFocus();
	}
}
