// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	_MaxHealth = 100.0f;
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

	UE_LOG(LogTemp, Display, TEXT("Damaged for %f, %f health remaining"), change, _CurrentHealth);
	if(_CurrentHealth == 0.0f)
	{
		UE_LOG(LogTemp, Display, TEXT("Dead time"));
	}
}

