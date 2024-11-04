#include "Systems/System.h"

USystem::USystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	_MaxValue = 100.0f;
	_CurrentValue = 0.0f;

	_ValueChangeAmount = 0.0f;
	_ValueChangeTime = 0.0f;
	_TimerManager = nullptr;
}

void USystem::BeginPlay()
{
	Super::BeginPlay();
	_TimerManager = &GetWorld()->GetTimerManager();
	_CurrentValue = _MaxValue;

	
	SetFaultTimer();
}

void USystem::SetFaultTimer()
{
	_TimerManager->SetTimer(_StartFaultTimer, this, &USystem::Handle_FaultStarted,
		FMath::RandRange(_MinFaultTime, _MaxFaultTime), false);
}

void USystem::Handle_FaultStarted()
{
	if(!_TimerManager->IsTimerActive(_ValueChangeTimer)) { _TimerManager->ClearTimer(_ValueChangeTimer); }
	
	_TimerManager->SetTimer(_ValueChangeTimer, this, &USystem::Handle_ValueChanged,
	_ValueChangeTime, true);
}

void USystem::Handle_FaultStopped()
{
	if(!_TimerManager->IsTimerActive(_ValueChangeTimer)) { _TimerManager->ClearTimer(_ValueChangeTimer); }

	_TimerManager->SetTimer(_ValueChangeTimer, this, &USystem::Handle_Regenerated,
_ValueChangeTime, true);
}

void USystem::Handle_ValueChanged()
{
	_CurrentValue -= _ValueChangeAmount;

	if(_CurrentValue <= 0)
	{
		_CurrentValue = 0;
		_TimerManager->ClearTimer(_ValueChangeTimer);
		OnDepleted.Broadcast();
	}
	
	OnValueChanged.Broadcast(_MaxValue, _CurrentValue);
}

void USystem::Handle_Regenerated()
{
	_CurrentValue += _ValueChangeAmount;

	if(_CurrentValue >= _MaxValue)
	{
		_CurrentValue = _MaxValue;
		_TimerManager->ClearTimer(_ValueChangeTimer);
	}

	if(!_TimerManager->IsTimerActive(_ValueChangeTimer)) { SetFaultTimer(); }
}
