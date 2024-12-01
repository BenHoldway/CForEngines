#include "Game Managers/GameRule_Systems.h"

#include "LightObject.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/System.h"
#include "Systems/SystemDisplayer.h"
#include "Systems/System_Controller.h"


UGameRule_Systems::UGameRule_Systems()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGameRule_Systems::Init()
{
	Super::Init();

	for(ASystem_Controller* systemController : _Systems)
	{
		systemController->Init();
	}
}

void UGameRule_Systems::BeginPlay()
{
	Super::BeginPlay();
	ASystem_Controller::OnSystemRegister.AddUniqueDynamic(this, &UGameRule_Systems::Handle_SystemRegistered);
	ASystemDisplayer::OnRegister.AddUniqueDynamic(this, &UGameRule_Systems::Handle_SystemDisplayRegistered);
	ALightObject::OnRegister.AddUniqueDynamic(this, &UGameRule_Systems::Handle_LightRegistered);
}

void UGameRule_Systems::Handle_SystemRegistered(ASystem_Controller* systemController)
{
	if(_Systems.Contains(systemController)) { return; }

	_Systems.Add(systemController);
	systemController->OnDepleted.AddUniqueDynamic(this, &UGameRule_Systems::Handle_SystemDepleted);
	systemController->OnSystemValueChanged.AddUniqueDynamic(this, &UGameRule_Systems::Handle_SystemValueChanged);
	systemController->OnSystemStateChanged.AddUniqueDynamic(this, &UGameRule_Systems::Handle_SystemStateChanged);
}

void UGameRule_Systems::Handle_LightRegistered(ALightObject* light)
{
	_Lights.AddUnique(light);
}

void UGameRule_Systems::Handle_SystemDisplayRegistered(ASystemDisplayer* systemDisplayer)
{
	_SystemDisplay = systemDisplayer;
}

void UGameRule_Systems::Handle_SystemValueChanged(ESystemType systemType, float max, float current)
{
	switch (systemType)
	{
	case ESystemType::Power:
		_SystemDisplay->UpdatePower(max, current);
		break;
	case ESystemType::Oxygen:
		_SystemDisplay->UpdateOxygen(max, current);
		break;
	default:
		break;
	}
}


void UGameRule_Systems::Handle_SystemDepleted(ASystem_Controller* systemController, ESystemType systemType)
{
	switch (systemType)
	{
	case ESystemType::Power:
		for(ALightObject* lightActor : _Lights)
		{
			if(UKismetSystemLibrary::DoesImplementInterface(lightActor, UTurnoffable::StaticClass()))
			{
				ITurnoffable::Execute_TurnOff(lightActor);
			}
		}
		break;
	case ESystemType::Oxygen:
		UE_LOG(LogTemp, Display, TEXT("Dead"));
		OnSystemDepleted.Broadcast(systemType);
		break;
	default:
		break;
	}
	
}

void UGameRule_Systems::Handle_SystemStateChanged(ASystem_Controller* systemController, ESystemType systemType,
	bool isOn)
{
	switch (systemType)
	{
	case ESystemType::Power:
		if(!isOn) { _SystemDisplay->StopPowerAlarm(); }
		break;
	case ESystemType::Oxygen:
		if(!isOn) { _SystemDisplay->StopOxygenAlarm(); }
		break;
	default:
		break;
	}
}

void UGameRule_Systems::UpdateClock(int hours, int minutes)
{
	_SystemDisplay->UpdateClock(hours, minutes);
}