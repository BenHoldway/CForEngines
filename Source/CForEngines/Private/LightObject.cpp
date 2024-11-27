#include "LightObject.h"

#include "Components/RectLightComponent.h"

ALightObject::ALightObject()
{
	PrimaryActorTick.bCanEverTick = false;

	_Light = CreateDefaultSubobject<URectLightComponent>(TEXT("Light"));
	_Light->SetupAttachment(RootComponent);
}

void ALightObject::BeginPlay()
{
	Super::BeginPlay();

	OnRegister.Broadcast(this);
	_Intensity = _Light->Intensity;
}

void ALightObject::TurnOff_Implementation()
{
	_Light->SetIntensity(0.0f);
	UE_LOG(LogTemp, Display, TEXT("Light: %f"), _Light->Intensity);
}

void ALightObject::TurnOn_Implementation()
{
	_Light->SetIntensity(_Intensity);
}

