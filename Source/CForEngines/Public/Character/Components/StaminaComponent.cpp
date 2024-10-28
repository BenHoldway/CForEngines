#include "Character/Components/StaminaComponent.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	_CurrentStamina = _MaxStamina;
	_TimerManager = &GetWorld()->GetTimerManager();
}

void UStaminaComponent::StartSprinting()
{
	_TimerManager->SetTimer(_SprintTimer, this, &UStaminaComponent::Sprint, _StaminaDrainInterval, true);
}

void UStaminaComponent::Sprint()
{
	//Clear regain timer
	if(_TimerManager->TimerExists(_SprintRegainTimer)) { _TimerManager->ClearTimer(_SprintRegainTimer); }

	//Reduce stamina
	_CurrentStamina -= 1.0f;
	_CurrentStamina = FMath::Clamp(_CurrentStamina, 0.0f, _MaxStamina);
	OnStaminaChanged.Broadcast(_CurrentStamina, _MaxStamina, 1.0f);

	if(_CurrentStamina <= 0.0f) { StopSprinting(1.0f); }
}

void UStaminaComponent::StopSprinting(float delay)
{
	_TimerManager->ClearTimer(_SprintTimer);
	_TimerManager->SetTimer(_SprintRegainTimer, this, &UStaminaComponent::RegainStamina, _StaminaRegainInterval, true, delay);

	OnStoppedSprinting.Broadcast();
}

void UStaminaComponent::RegainStamina()
{
	_CurrentStamina += 1.0f;
	_CurrentStamina = FMath::Clamp(_CurrentStamina, 0.0f, _MaxStamina);
	OnStaminaChanged.Broadcast(_CurrentStamina, _MaxStamina, 1.0f);
	
	if(_CurrentStamina == _MaxStamina)
	{
		_TimerManager->ClearTimer(_SprintRegainTimer);
	}
}

