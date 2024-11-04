#include "Systems/System_Controller.h"

#include "Components/BoxComponent.h"
#include "Systems/System.h"

ASystem_Controller::ASystem_Controller()
{
	PrimaryActorTick.bCanEverTick = false;

	_Collider2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = _Collider2;

	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_Mesh -> SetupAttachment(RootComponent);
	
	_System = CreateDefaultSubobject<USystem>(TEXT("System"));
}

void ASystem_Controller::BeginPlay()
{
	Super::BeginPlay();
	_System->OnValueChanged.AddUniqueDynamic(this, &ASystem_Controller::Handle_SystemValueChanged);
	_System->OnDepleted.AddUniqueDynamic(this, &ASystem_Controller::Handle_SystemDepleted);

	OnSystemRegister.Broadcast(this);
}

void ASystem_Controller::Interact_Implementation()
{
	_System->Handle_FaultStopped();
}

void ASystem_Controller::Handle_SystemValueChanged(float max, float current)
{
	//TODO Update in Widget
}

void ASystem_Controller::Handle_SystemRegenerated()
{
	_System->Handle_Regenerated();
}

void ASystem_Controller::Handle_SystemDepleted()
{
	//TODO Broadcast to GM that system has been depleted
}

