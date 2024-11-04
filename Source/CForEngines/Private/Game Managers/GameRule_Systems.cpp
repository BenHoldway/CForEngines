#include "Game Managers/GameRule_Systems.h"

#include "Systems/System.h"
#include "Systems/System_Controller.h"


UGameRule_Systems::UGameRule_Systems()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGameRule_Systems::Init()
{
	Super::Init();

	ASystem_Controller::OnSystemRegister.AddUniqueDynamic(this, &UGameRule_Systems::Handle_SystemRegistered);
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
}

void UGameRule_Systems::Handle_SystemDepleted(ASystem_Controller* systemController, USystem* system)
{
	OnSystemDepleted.Broadcast(system);
}

