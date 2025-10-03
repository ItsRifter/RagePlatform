// Fill out your copyright notice in the Description page of Project Settings.


#include "World/PatrolNodes.h"

// Sets default values
APatrolNode::APatrolNode()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

// Called when the game starts or when spawned
void APatrolNode::BeginPlay()
{
	Super::BeginPlay();

	//Check for issues and make log messages where necessary
	if (!NextNode)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing a patrol node to proceed next"), *GetName());
		return;
	} 
	else if (!CastChecked<APatrolNode>(NextNode))
	{
		UE_LOG(LogTemp, Error, TEXT("The next node for %s is not a patrol node"), *GetName());
		return;
	}

	APatrolNode* node = Cast<APatrolNode>(NextNode);
	
	if (!node->BackNode)
	{
		node->BackNode = this;
	}
	
	if (OptionalNode && !CastChecked<APatrolNode>(OptionalNode))
	{
		UE_LOG(LogTemp, Error, TEXT("The optional node for %s is not a patrol node"), *GetName());
	}
}
