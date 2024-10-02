// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstActor.h"

#include "Components/BoxComponent.h"
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

void AFirstActor::Handle_ColliderHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Display, TEXT("I hit"));
	
}

// Called when the game starts or when spawned
void AFirstActor::BeginPlay()
{
	Super::BeginPlay();

	_Collider->OnComponentHit.AddUniqueDynamic(this, &AFirstActor::Handle_ColliderHit);
	
}

// Called every frame
void AFirstActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

