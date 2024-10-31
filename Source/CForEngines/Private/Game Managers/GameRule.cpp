#include "Game Managers/GameRule.h"

UGameRule::UGameRule()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGameRule::Init()
{
}

void UGameRule::BroadcastComplete(bool successful)
{
	OnComplete.Broadcast(successful);
}

void UGameRule::BroadcastPointsScored(AController* scorer, int points)
{
	OnPointsScored.Broadcast(scorer, points);
}

