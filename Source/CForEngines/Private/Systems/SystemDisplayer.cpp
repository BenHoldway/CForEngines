#include "Systems/SystemDisplayer.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
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

void ASystemDisplayer::UpdatePower(float max, float current)
{
	UE_LOG(LogTemp, Display, TEXT("Attempting to Update Power"));
	if(UKismetSystemLibrary::DoesImplementInterface(_SystemWidget, USystemable::StaticClass()))
	{
		UE_LOG(LogTemp, Display, TEXT("Updating Power"));
		ISystemable::Execute_UpdatePower(_SystemWidget, current/max);
	}
}

void ASystemDisplayer::UpdateOxygen(float max, float current)
{
	UE_LOG(LogTemp, Display, TEXT("Attempting to Update Oxygen"));
	if(UKismetSystemLibrary::DoesImplementInterface(_SystemWidget, USystemable::StaticClass()))
	{
		UE_LOG(LogTemp, Display, TEXT("Updating Oxygen"));
		ISystemable::Execute_UpdateOxygen(_SystemWidget, current/max);
	}
}

void ASystemDisplayer::BeginPlay()
{
	Super::BeginPlay();

	_SystemWidget = Cast<USystemWidget>(_SystemWidgetComponent->GetUserWidgetObject());
	_SystemWidget->NativeConstruct();

	OnRegister.Broadcast(this);
}

