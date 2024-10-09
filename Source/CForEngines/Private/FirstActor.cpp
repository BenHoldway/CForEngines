#include "FirstActor.h"

#include "HealthComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "GameFramework/RotatingMovementComponent.h"


AFirstActor::AFirstActor()
{
	_Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = _Collider;

	_Meesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_Meesh -> SetupAttachment(RootComponent);

	_Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	_Arrow -> SetupAttachment(RootComponent);

	//_Rotate = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotate"));
	
	PrimaryActorTick.bCanEverTick = true;
}

void AFirstActor::BeginPlay()
{
	Super::BeginPlay();

	_Collider->OnComponentHit.AddUniqueDynamic(this, &AFirstActor::Handle_ColliderHit);
	
}

void AFirstActor::Handle_ColliderHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	TObjectPtr<UHealthComponent> _Health = OtherActor->FindComponentByClass<UHealthComponent>();
	if(_Health == nullptr) { return; }
	UGameplayStatics::ApplyDamage(this, 10.f, nullptr, this, UDamageType::StaticClass());
}

