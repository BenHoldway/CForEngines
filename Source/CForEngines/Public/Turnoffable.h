// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Turnoffable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UTurnoffable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CFORENGINES_API ITurnoffable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TurnOff();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TurnOn();
};
