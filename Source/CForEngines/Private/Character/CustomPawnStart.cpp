#include "Character/CustomPawnStart.h"

void ACustomPawnStart::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ACustomPawnStart::GetIsAI()
{
	return _IsAI;
}

