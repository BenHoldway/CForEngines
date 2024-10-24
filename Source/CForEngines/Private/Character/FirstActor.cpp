#include "Character/FirstActor.h"

#include "Character/Components/HealthComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


class UHealthComponent;

AFirstActor::AFirstActor()
{
	_Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = _Collider;
	_Collider->OnComponentHit.AddUniqueDynamic(this, &AFirstActor::Handle_ColliderHit);

	_Meesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_Meesh -> SetupAttachment(RootComponent);

	_Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	_Arrow -> SetupAttachment(RootComponent);
	
	PrimaryActorTick.bCanEverTick = true;
}

void AFirstActor::Handle_ColliderHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		UGameplayStatics::ApplyDamage(OtherActor, 10.f, nullptr, this, UDamageType::StaticClass());
	}
}

