// Fill out your copyright notice in the Description page of Project Settings.


#include "World/RLevelChangeActor.h"

#include "Components/BoxComponent.h"
#include "Framework/RGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RageCharacter.h"

// Sets default values
ARLevelChangeActor::ARLevelChangeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot"));

	PlayerOverlap = CreateDefaultSubobject<UBoxComponent>("PlayerOverlap");
	PlayerOverlap->SetupAttachment(GetRootComponent());

	NextLevel = NAME_None;
	FadeDelay = .5f;
	bLevelChanged = false;
}

// Called when the game starts or when spawned
void ARLevelChangeActor::BeginPlay()
{
	Super::BeginPlay();

	bLevelChanged = false;
	PlayerController = UGameplayStatics::GetPlayerController(this,0);
	PlayerCharacter = Cast<ARageCharacter>(UGameplayStatics::GetPlayerCharacter(this,0));

	GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	PlayerOverlap->OnComponentBeginOverlap.AddDynamic(this,&ARLevelChangeActor::OnComponentBeginOverlap);
}

void ARLevelChangeActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                 const FHitResult& SweepResult)
{
	if (bLevelChanged)
	{
		return;
	}

	if (Cast<ARageCharacter>(OtherActor))
	{
		GetWorld()->GetTimerManager().SetTimer(
			FadeHandle,
			this, &ARLevelChangeActor::OpenNextLevel,
			FadeDelay,
			false);

		if (PlayerController)
		{
			if (PlayerCharacter)
			{
				PlayerCharacter->GetMovementComponent()->StopMovementImmediately();
				PlayerCharacter->GetCharacterMovement()->MaxAcceleration = 0.f;
			}
			
			PlayerController->PlayerCameraManager->StartCameraFade(
				0.f,
				1.f,
				FadeDelay,
				FLinearColor::Black,
				false,
				true);
		}
	}
}

void ARLevelChangeActor::OpenNextLevel()
{
	if (NextLevel != NAME_None)
	{
		UGameplayStatics::OpenLevel(this,NextLevel);
		GameInstance->TimeVar = 0;
		GameInstance->bCanCountLevelTime = false;
		GameInstance->bCanCountGameTime = false;
	}
}

