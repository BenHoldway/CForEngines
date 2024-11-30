#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Components/Controllable.h"
#include "Components/Resetable.h"
#include "GameFramework/PlayerController.h"
#include "PC_FPS.generated.h"

class UWidget_HUD;
struct FInputActionValue;
class UInputAction;
class UGM_Widget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDeadSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDamagedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPauseGameSignature, APlayerController*, playerController);

UCLASS(Abstract)
class CFORENGINES_API APC_FPS : public APlayerController, public IControllable, public IGenericTeamAgentInterface, public IResetable
{
	GENERATED_BODY()
	
public:
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void Reset_Implementation(FVector pos) override;

	virtual void DisablePlayerInput_Implementation() override;
	virtual void EnablePlayerInput_Implementation() override;

	FPlayerDeadSignature OnPlayerDead;
	FPlayerDamagedSignature OnPlayerDamaged;
	FPauseGameSignature OnPauseGame;
	
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

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> _FlashlightToggleAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> _PauseGameAction;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWidget_HUD> _HUDWidgetClass;
	TObjectPtr<UWidget_HUD> _HUDWidget;

	int _Score;
	
	bool _IsCrouching;
	bool _IsMoving;
	bool _IsSprinting;
	bool _IsFlashlightOn;

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
	void FlashlightToggle();
	void PauseGameToggle();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void AddPoints_Implementation(int points) override;

	UFUNCTION()
	void Dead(AController* causer);

	UFUNCTION()
	void Damaged(float currentHealth, float maxHealth, float changedHealth);

	UFUNCTION()
	void StaminaChanged(float currentStamina, float maxStamina, float changedStamina);

	UFUNCTION()
	void ShowInteractPrompt();
	UFUNCTION()
	void HideInteractPrompt();

};

