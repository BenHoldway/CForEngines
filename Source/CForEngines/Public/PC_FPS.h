#pragma once

#include "CoreMinimal.h"
#include "Controllable.h"
#include "GameFramework/PlayerController.h"
#include "PC_FPS.generated.h"

class UWidget_HUD;
struct FInputActionValue;
class UInputAction;

UCLASS(Abstract)
class CFORENGINES_API APC_FPS : public APlayerController, public IControllable
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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWidget_HUD> _HUDWidgetClass;
	TObjectPtr<UWidget_HUD> _HUDWidget;

	int _Score;

	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void JumpPressed();
	void JumpReleased();
	void FirePressed();
	void FireReleased();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void AddPoints_Implementation(int points) override;

	UFUNCTION()
	void Damaged(float currentHealth, float maxHealth, float changedHealth);
};
