#include "Systems/System.h"

#include "Systems/System_Controller.h"

USystem::USystem()
{
	PrimaryComponentTick.bCanEverTick = false;
	_MaxValue = 100.0f;
	_CurrentValue = 0.0f;

	_ValueChangeAmount = 0.0f;
	_ValueChangeTime = 0.0f;
	_TimerManager = nullptr;
}

void USystem::Init(ASystem_Controller* owner)
{
	_Controller = owner;
}

void USystem::BeginPlay()
{
	Super::BeginPlay();
	_TimerManager = &GetWorld()->GetTimerManager();
	_CurrentValue = _MaxValue;
	_FaultHasStarted = false;

	
	SetFaultTimer();
}

void USystem::SetFaultTimer()
{
	_TimerManager->SetTimer(_StartFaultTimer, this, &USystem::Handle_FaultStarted,
		FMath::RandRange(_MinFaultTime, _MaxFaultTime), false);
}

void USystem::Handle_FaultStarted()
{
	UE_LOG(LogTemp, Display, TEXT("Fault started from %s"), *GetOwner()->GetName());
	_FaultHasStarted = true;
	
	if(!_TimerManager->IsTimerActive(_ValueChangeTimer)) { _TimerManager->ClearTimer(_ValueChangeTimer); }
	
	_TimerManager->SetTimer(_ValueChangeTimer, this, &USystem::Handle_ValueChanged,
	_ValueChangeTime, true);
}

void USystem::Handle_FaultStopped()
{
	UE_LOG(LogTemp, Display, TEXT("Fault stopped at %s"), *GetOwner()->GetName());
	UE_LOG(LogTemp, Display, TEXT("Fault stopped at %hhd"), _FaultHasStarted);
	_FaultHasStarted = false;
	
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

	OnValueChanged.Broadcast(_MaxValue, _CurrentValue);

	if(!_TimerManager->IsTimerActive(_ValueChangeTimer)) { SetFaultTimer(); }
}

bool USystem::HasFaultStarted()
{
	return _FaultHasStarted;
}

ESystemType USystem::GetSystemType()
{
	return _SystemType;
}
