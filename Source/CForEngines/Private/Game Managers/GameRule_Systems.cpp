#include "Game Managers/GameRule_Systems.h"

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

	ASystem_Controller::OnSystemRegister.AddUniqueDynamic(this, &UGameRule_Systems::Handle_SystemRegistered);
	ASystemDisplayer::OnRegister.AddUniqueDynamic(this, &UGameRule_Systems::Handle_SystemDisplayRegistered);
}

void UGameRule_Systems::BeginPlay()
{
	Super::BeginPlay();
	
}

void UGameRule_Systems::Handle_SystemRegistered(ASystem_Controller* systemController)
{
	if(_Systems.Contains(systemController)) { return; }

	_Systems.Add(systemController);
	systemController->OnDepleted.AddUniqueDynamic(this, &UGameRule_Systems::Handle_SystemDepleted);

	for(ASystem_Controller* controller : _Systems)
	{
		controller->OnSystemValueChanged.AddUniqueDynamic(this, &UGameRule_Systems::Handle_SystemValueChanged);
	}
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
		UE_LOG(LogTemp, Display, TEXT("Power Updated for Game Rule"));
		_SystemDisplay->UpdatePower(max, current);
		break;
	case ESystemType::Oxygen:
		UE_LOG(LogTemp, Display, TEXT("Oxygen Updated for Game Rule"));
		_SystemDisplay->UpdateOxygen(max, current);
		break;
	default:
		break;
	}
}


void UGameRule_Systems::Handle_SystemDepleted(ASystem_Controller* systemController, ESystemType systemType)
{
	OnSystemDepleted.Broadcast(systemType);
}

