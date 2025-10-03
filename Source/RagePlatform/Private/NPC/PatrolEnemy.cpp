// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BoxComponent.h"
#include "Components/InterpToMovementComponent.h"
#include "NPC/PatrolEnemy.h"
#include "World/PatrolNodes.h"

// Sets default values
APatrolEnemy::APatrolEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	PathChecker = CreateDefaultSubobject<UBoxComponent>("PathChecker");
	PathChecker->SetupAttachment(GetRootComponent());

	MoveComponent = CreateDefaultSubobject<UInterpToMovementComponent>("Mover");
	MoveComponent->bSweep = true;
	MoveComponent->BehaviourType = EInterpToBehaviourType::OneShot;

	MoveSpeed = 1.0f;
	MoveDuration = 3.0f;
}

// Called when the game starts or when spawned
void APatrolEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	MoveComponent->SpeedMultiplier = MoveSpeed;
	MoveComponent->Duration = MoveDuration;

	PathChecker->OnComponentBeginOverlap.AddDynamic(this, &APatrolEnemy::OnNodePass);

	if (!StartNode)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Is missing a start node"), *GetName())
		return;
	}

	APatrolNode* node = Cast<APatrolNode>(StartNode);

	if (!node)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Is missing a start node"), *GetName())
		return;
	}

	SetActorLocation(node->GetActorLocation());

	if (!node->NextNode)
	{
		return;
	}

	LastNode = node;
	TargetNode = node->NextNode;

	MoveComponent->ResetControlPoints();

	//Get the last node for a control point otherwise the actor just stays in place
	MoveComponent->AddControlPointPosition(LastNode->GetActorLocation(), false);
	MoveComponent->AddControlPointPosition(TargetNode->GetActorLocation(), false);
	
	MoveComponent->RestartMovement();
	MoveComponent->FinaliseControlPoints();
}

void APatrolEnemy::OnNodePass(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!TargetNode)
	{
		return;
	}

	APatrolNode* Node = Cast<APatrolNode>(OtherActor);

	if (Node == TargetNode)
	{
		// If optional node exists, see if actor should take that path
		if (Node->OptionalNode)
		{
			if (LastNode != Node->OptionalNode && FMath::RandRange(1, 2) == 2)
			{
				TargetNode = Node->OptionalNode;
			}
			else
			{
				TargetNode = Node->NextNode;
			}
		}
		else
		{
			if (Node->NextNode)
			{
				TargetNode = Node->NextNode;
			}
		}

		if (!CheckPathing(TargetNode, LastNode) && Node->BackNode)
		{
			TargetNode = Node->BackNode;
		}
	
		LastNode = Node;

		if (Node->HoldPositionTime > 0)
		{
			MoveComponent->Deactivate();

			GetWorld()->GetTimerManager().SetTimer(WaitHandle, this,
				&APatrolEnemy::MoveToNext, Node->HoldPositionTime, false);
		}
		else
		{
			MoveToNext();
		}
	}
}

void APatrolEnemy::MoveToNext()
{
	MoveComponent->Activate();

	MoveComponent->ResetControlPoints();

	MoveComponent->AddControlPointPosition(LastNode->GetActorLocation(), false);
	MoveComponent->AddControlPointPosition(TargetNode->GetActorLocation(), false);

	MoveComponent->FinaliseControlPoints();
	MoveComponent->RestartMovement();
}

bool APatrolEnemy::CheckPathing(AActor* Goto, AActor* From)
{
	APatrolNode* CheckA = Cast<APatrolNode>(Goto);
	APatrolNode* CheckB = Cast<APatrolNode>(From);

	//Target is going back to where this actor previously came from
	if (CheckA == CheckB)
	{
		return false;
	}

	return true;
}