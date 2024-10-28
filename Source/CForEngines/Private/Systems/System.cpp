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

	SetTimer();
}

void USystem::SetTimer()
{
	_TimerManager->SetTimer(_ValueChangeTimer, this, &USystem::Handle_ValueChanged, _ValueChangeTime, true);
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
	_CurrentValue = _MaxValue;

	if(!_TimerManager->IsTimerActive(_ValueChangeTimer)) { SetTimer(); }
}
