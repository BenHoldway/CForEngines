#include "Game Managers/GameRule_Target.h"

#include "Character/Target.h"

UGameRule_Target::UGameRule_Target()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGameRule_Target::Init()
{
	Super::Init();

	UTarget::OnTargetRegister.AddUniqueDynamic(this, &UGameRule_Target::Handle_TargetRegistered);
	OnRequestTargets.Broadcast();
}

void UGameRule_Target::Handle_TargetRegistered(UTarget* target)
{
	if(_Targets.Contains(target)) { return; }

	_Targets.Add(target);
	target->OnDestroyed.AddUniqueDynamic(this, &UGameRule_Target::Handle_TargetDestroyed);
	_AmountRemaining++;
}

void UGameRule_Target::Handle_TargetDestroyed(UTarget* target, AController* causer, int targetValue)
{
	_Targets.Remove(target);
	_AmountRemaining--;
	BroadcastPointsScored(causer, targetValue);

	if(_AmountRemaining == 0) { BroadcastComplete(); }
}

