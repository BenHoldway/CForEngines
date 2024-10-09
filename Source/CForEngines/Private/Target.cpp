#include "CForEngines/Public/Target.h"

#include "HealthComponent.h"


UTarget::UTarget()
{
	PrimaryComponentTick.bCanEverTick = false;
	_PointsValue = 1;
}

void UTarget::BeginPlay()
{
	Super::BeginPlay();

	AActor* owner = GetOwner();
	_Health = owner->FindComponentByClass<UHealthComponent>();
	if(_Health == nullptr)
	{
		_Health = NewObject<UHealthComponent>(owner, TEXT("Health"));
		owner->AddInstanceComponent(_Health);
		_Health->RegisterComponent();
		owner->Modify();
	}
	_Health->OnDead.AddUniqueDynamic(this, &UTarget::Handle_HealthDead);

	//TODO: Add bind to gamerule_target request for census

	OnTargetRegister.Broadcast(this);
}

void UTarget::Handle_HealthDead(AController* causer)
{
	OnDestroyed.Broadcast(this, causer, _PointsValue);
	GetOwner()->Destroy();
}

void UTarget::Handle_GameRuleRequestTargets()
{
	OnTargetRegister.Broadcast(this);
}

