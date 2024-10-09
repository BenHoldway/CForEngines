// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Target.generated.h"

class UHealthComponent;
class UTarget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTargetDestroyedSignature, UTarget*, target,
	AController*, destroyer, int, targetValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetRegisteredSignature, UTarget*, target);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CFORENGINES_API UTarget : public UActorComponent
{
	GENERATED_BODY()

public:
	UTarget();

	UPROPERTY(BlueprintAssignable)
	FTargetDestroyedSignature OnDestroyed;

	static inline FTargetRegisteredSignature OnTargetRegister;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int _PointsValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHealthComponent> _Health;
	
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void Handle_HealthDead(AController* causer);
	UFUNCTION()
	void Handle_GameRuleRequestTargets();
};
