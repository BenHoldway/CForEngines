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
	_SystemWidgetComponent->AttachToComponent(_Collider, FAttachmentTransformRules::KeepRelativeTransform);
}

void ASystemDisplayer::BeginPlay()
{
	Super::BeginPlay();

	_SystemWidget = Cast<USystemWidget>(_SystemWidgetComponent->GetUserWidgetObject());
	_SystemWidget->NativeConstruct();
}

