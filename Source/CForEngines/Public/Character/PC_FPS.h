#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Components/Controllable.h"
#include "GameFramework/PlayerController.h"
#include "PC_FPS.generated.h"

class UWidget_HUD;
struct FInputActionValue;
class UInputAction;

UCLASS(Abstract)
class CFORENGINES_API APC_FPS : public APlayerController, public IControllable, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
	
protected:	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> _MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> _SprintAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> _LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> _JumpAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> _FireAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> _CrouchAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> _InteractAction;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWidget_HUD> _HUDWidgetClass;
	TObjectPtr<UWidget_HUD> _HUDWidget;

	int _Score;
	
	bool _IsCrouching;
	bool _IsMoving;
	bool _IsSprinting;

	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& value);
	void SprintPressed();
	void SprintReleased();
	void Look(const FInputActionValue& value);
	void JumpPressed();
	void JumpReleased();
	void FirePressed();
	void FireReleased();
	void CrouchPressed();
	void CrouchReleased();
	void Interact();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void AddPoints_Implementation(int points) override;

	UFUNCTION()
	void Damaged(float currentHealth, float maxHealth, float changedHealth);

	UFUNCTION()
	void StaminaChanged(float currentStamina, float maxStamina, float changedStamina);

	UFUNCTION()
	void ShowInteractPrompt();
	UFUNCTION()
	void HideInteractPrompt();

public:
	virtual FGenericTeamId GetGenericTeamId() const override;
};

