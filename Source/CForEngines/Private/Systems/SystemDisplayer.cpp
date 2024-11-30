#include "Systems/SystemDisplayer.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Systems/SystemWidget.h"

ASystemDisplayer::ASystemDisplayer()
{
	PrimaryActorTick.bCanEverTick = false;

	_Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = _Collider;

	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_Mesh -> SetupAttachment(RootComponent);

	_SystemWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("System Widget"));
	_SystemWidgetComponent->SetupAttachment(_Collider);
}

void ASystemDisplayer::BeginPlay()
{
	Super::BeginPlay();

	_SystemWidget = Cast<USystemWidget>(_SystemWidgetComponent->GetUserWidgetObject());
	_SystemWidget->NativeConstruct();

	OnRegister.Broadcast(this);
}

void ASystemDisplayer::UpdatePower(float max, float current)
{
	_SystemWidget->UpdatePower(current/max);
}

void ASystemDisplayer::UpdateOxygen(float max, float current)
{
	_SystemWidget->UpdateOxygen(current/max);
}

void ASystemDisplayer::StopPowerAlarm()
{
	_SystemWidget->StopPowerAlarm();
}

void ASystemDisplayer::StopOxygenAlarm()
{
	_SystemWidget->StopOxygenAlarm();
}

