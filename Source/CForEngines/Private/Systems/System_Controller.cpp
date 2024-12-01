#include "Systems/System_Controller.h"

#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Systems/System.h"

ASystem_Controller::ASystem_Controller()
{
	PrimaryActorTick.bCanEverTick = false;

	_Collider2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = _Collider2;

	_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_Mesh -> SetupAttachment(RootComponent);

	_Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	_Light -> SetupAttachment(RootComponent);
	
	_System = CreateDefaultSubobject<USystem>(TEXT("System"));
}

void ASystem_Controller::Init()
{
	_System->Init(this);
	_System->OnValueChanged.AddUniqueDynamic(this, &ASystem_Controller::Handle_SystemValueChanged);
	_System->OnDepleted.AddUniqueDynamic(this, &ASystem_Controller::Handle_SystemDepleted);
	_System->OnFaultStateChanged.AddUniqueDynamic(this, &ASystem_Controller::Handle_SystemStateChanged);

	_SystemType = _System->GetSystemType();
}

void ASystem_Controller::BeginPlay()
{
	Super::BeginPlay();
	
	OnSystemRegister.Broadcast(this);
	
}

void ASystem_Controller::Interact_Implementation()
{
	if(_System->HasFaultStarted()) { _System->Handle_FaultStopped(); }
}

void ASystem_Controller::Handle_SystemValueChanged(float max, float current)
{
	OnSystemValueChanged.Broadcast(_SystemType, max, current);
}

void ASystem_Controller::Handle_SystemRegenerated()
{
	_System->Handle_Regenerated();
}

void ASystem_Controller::Handle_SystemDepleted()
{
	OnDepleted.Broadcast(this, _SystemType);
}

void ASystem_Controller::Handle_SystemStateChanged(bool isOn)
{
	OnSystemStateChanged.Broadcast(this, _SystemType, isOn);
	if(isOn) { _Light->SetLightColor(_FaultColour); }
	else { _Light->SetLightColor(_NormalColour); }
}

