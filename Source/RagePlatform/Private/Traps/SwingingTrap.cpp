#include "Traps/SwingingTrap.h"

ASwingingTrap::ASwingingTrap()
{
	SwingDistance = 75.0f;
	SwingSpeed = 25.0f;
}

void ASwingingTrap::BeginPlay()
{
	Super::BeginPlay();
}