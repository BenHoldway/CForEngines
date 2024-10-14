#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_FPS.generated.h"

struct FInputActionValue;
class UInputAction;

UCLASS(Abstract)
class CFORENGINES_API APC_FPS : public APlayerController
{
	GENERATED_BODY()

protected:	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> _MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> _LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> _JumpAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> _FireAction;

	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void JumpPressed();
	void JumpReleased();
	void FirePressed();
	void FireReleased();

	virtual void OnPossess(APawn* InPawn) override;
};
