// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_Debug.generated.h"

struct FInputActionValue;
class UInputAction;

UCLASS(Abstract)
class CFORENGINES_API APC_Debug : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> _DebugAction;

	virtual void SetupInputComponent() override;

	void Debug(const FInputActionValue& Value);
};
