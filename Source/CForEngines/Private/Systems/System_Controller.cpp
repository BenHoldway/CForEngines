#include "Systems/System_Controller.h"

#include "Components/SphereComponent.h"
#include "Systems/System.h"

ASystem_Controller::ASystem_Controller()
{
	PrimaryActorTick.bCanEverTick = false;

	_Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	RootComponent = _Collider;

	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_Mesh -> SetupAttachment(RootComponent);
	
	_System = CreateDefaultSubobject<USystem>(TEXT("System"));
}

void ASystem_Controller::BeginPlay()
{
	Super::BeginPlay();
	_System->OnValueChanged.AddUniqueDynamic(this, &ASystem_Controller::Handle_SystemValueChanged);
	_System->OnDepleted.AddUniqueDynamic(this, &ASystem_Controller::Handle_SystemDepleted);
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

