#include "HealthComponent.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	_MaxHealth = 100.0f;
	_CurrentHealth = 0.0f;
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	_CurrentHealth = _MaxHealth;
	GetOwner()->OnTakeAnyDamage.AddUniqueDynamic(this, &UHealthComponent::DamageTaken);
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	const float change = FMath::Min(_CurrentHealth, Damage);
	_CurrentHealth -= change;

	OnDamaged.Broadcast(_CurrentHealth, _MaxHealth, change);
	
	if(_CurrentHealth == 0.0f)
	{
		OnDead.Broadcast(InstigatedBy);
		GetOwner()->Destroy();
	}
}

