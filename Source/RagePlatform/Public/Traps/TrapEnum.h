#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETrap : uint8
{
	Spikes				UMETA(DisplayName = "Spikes"),
	Axe					UMETA(DisplayName = "SwingingAxe"),
	Saw					UMETA(DisplayName = "Sawblade"),
	Chandelier			UMETA(DisplayName = "Chandelier"),
	LavaPit				UMETA(DisplayName = "LavaPit"),
	PoisonPit			UMETA(DisplayName = "PoisonPit"),
	Explosion			UMETA(DisplayName = "Explosions"),
	Arrows				UMETA(DisplayName = "Arrows"),
};
