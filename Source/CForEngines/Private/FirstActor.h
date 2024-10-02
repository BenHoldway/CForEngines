// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
//#include "GameFramework/RotatingMovementComponent.h"
#include "FirstActor.generated.h"

class UBoxComponent;
class UArrowComponent;
class URotatingMovementComponent;

UCLASS()
class CFORENGINES_API AFirstActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFirstActor();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> _Meesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UArrowComponent> _Arrow;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//TObjectPtr<URotatingMovementComponent> _Rotate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> _Collider;

	UFUNCTION()
	void Handle_ColliderHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
