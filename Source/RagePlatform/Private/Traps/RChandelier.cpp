#include "Traps/RChandelier.h"

#include "TrapEnums.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Framework/RGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/RageCharacter.h"
#include "Player/RTempCamera.h"

// Sets default values
ARChandelier::ARChandelier()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot"));

	ChandelierMesh = CreateDefaultSubobject<UStaticMeshComponent>("ChandelierMesh");
	ChandelierMesh->SetupAttachment(GetRootComponent());

	KillBox = CreateDefaultSubobject<UBoxComponent>("KillBox");
	KillBox->SetupAttachment(ChandelierMesh);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
	AudioComponent->SetupAttachment(ChandelierMesh);

	PlayerOverlapBox = CreateDefaultSubobject<UBoxComponent>("PlayerOverlap");
	PlayerOverlapBox->SetupAttachment(GetRootComponent());

	FallSpeed = 7;
	KillTexts.Add(FText::FromString(TEXT("Chandelier Fell On Your Head!!")));
}

// Called when the game starts or when spawned
void ARChandelier::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<URGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnDeath.AddDynamic(this, &ARChandelier::OnDeathDelegate);
		GameInstance->OnGameRestart.AddDynamic(this, &ARChandelier::OnRestartDelegate);
	}

	KillBox->OnComponentBeginOverlap.AddDynamic(this, &ARChandelier::OnComponentBeginOverlapKillBox);
	PlayerOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ARChandelier::OnComponentBeginOverlapPlayerBox);

	ChandelierMesh->SetSimulatePhysics(false);

	StartLocation = ChandelierMesh->GetComponentLocation();
	StartRotation = ChandelierMesh->GetComponentRotation();
}

void ARChandelier::OnComponentBeginOverlapKillBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
	if (!bChandelierFell)
	{
		return;
	}

	/*if (ARageCharacter* PlayerCharacter = Cast<ARageCharacter>(OtherActor))
	{
		if (!PlayerCharacter->bIsAlive)
		{
			return;
		}

		PlayerCharacter->bIsAlive = false;
		PlayerCharacter->OnDeath(ETrap::Chandelier);

		if (KillSound)
		{
			UGameplayStatics::PlaySound2D(this, KillSound);
		}

		const int32 Index = UKismetMathLibrary::RandomIntegerInRange(0,KillTexts.Num() - 1);
		
		GameInstance->OnDeath.Broadcast(KillTexts[Index]);
		PlayerCharacter->PlayerFall(FVector::ZeroVector);

		if (PlayerCharacter->Temp_Camera)
		{
			PlayerCharacter->Temp_Camera->FocusVar = this;
			PlayerCharacter->Temp_Camera->StartFocus();
		}
	}*/
}

void ARChandelier::OnComponentBeginOverlapPlayerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                    bool bFromSweep, const FHitResult& SweepResult)
{

	if (ARageCharacter* PlayerCharacter = Cast<ARageCharacter>(OtherActor))
	{	
		if (!PlayerCharacter->bIsAlive)
		{
			return;
		}

		if (ActivateSound)
		{
			AudioComponent->SetSound(ActivateSound);
			AudioComponent->Play();
		}

		ChandelierMesh->SetSimulatePhysics(true);

		const float ImpulseZ = UKismetMathLibrary::MapRangeClamped(
			FallSpeed,
			1.f,
			10.f,
			500.f,
			1500.f) * -1;

		ChandelierMesh->AddImpulse(FVector(0.f, 0.f, ImpulseZ), NAME_None, true);
		bChandelierFell = true;

		PlayerCharacter->bIsAlive = false;
		PlayerCharacter->OnDeath(ETrap::Chandelier);

		if (KillSound)
		{
			UGameplayStatics::PlaySound2D(this, KillSound);
		}

		const int32 Index = UKismetMathLibrary::RandomIntegerInRange(0, KillTexts.Num() - 1);

		GameInstance->OnDeath.Broadcast(KillTexts[Index]);
		PlayerCharacter->PlayerFall(FVector::ZeroVector);

		if (PlayerCharacter->Temp_Camera)
		{
			PlayerCharacter->Temp_Camera->FocusVar = this;
			PlayerCharacter->Temp_Camera->StartFocus();
		}
	}
}

void ARChandelier::OnDeathDelegate(const FText& DeathText)
{
	bChandelierFell = false;
}

void ARChandelier::OnRestartDelegate()
{
	ChandelierMesh->SetSimulatePhysics(false);
	ChandelierMesh->SetWorldLocation(StartLocation);
	ChandelierMesh->SetWorldRotation(StartRotation);
}
