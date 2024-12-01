#include "Systems/SystemDisplayer.h"

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
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

	_SystemWidget->OnPlayAlarmSound.AddUniqueDynamic(this, &ASystemDisplayer::PlayAlarmSound);
	_SystemWidget->OnStopAlarmSound.AddUniqueDynamic(this, &ASystemDisplayer::StopAlarmSound);

	OnRegister.Broadcast(this);
}

void ASystemDisplayer::PlayAlarmSound()
{
	_AlarmSoundComponent = UGameplayStatics::CreateSound2D(this, _AlarmSound);
	_AlarmSoundComponent->Play();
	GetWorld()->GetTimerManager().SetTimer(_AlarmTimer, this, &ASystemDisplayer::PlayAlarmSound, _AlarmSoundWaitTime);
}

void ASystemDisplayer::StopAlarmSound()
{
	if(_AlarmSoundComponent) { _AlarmSoundComponent->DestroyComponent(); }
	GetWorld()->GetTimerManager().ClearTimer(_AlarmTimer);
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

void ASystemDisplayer::UpdateClock(int hours, int minutes)
{
	_SystemWidget->UpdateClock(hours, minutes);
}

