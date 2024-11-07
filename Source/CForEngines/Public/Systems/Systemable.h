// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Systemable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USystemable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CFORENGINES_API ISystemable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdatePower(float newPowerRatio);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateOxygen(float newOxygenRatio);
};
