#pragma once

#include "CoreMinimal.h"
#include "Components/Inputable.h"
#include "GameFramework/Character.h"
#include "P_FPS.generated.h"

class UMovementComponent;
class AWeapon_Base;
class UHealthComponent;
class UStaminaComponent;
class UCharacterMovementComponent;
class UCameraComponent;
class UCapsuleComponent;
class UBehaviorTree;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPawnDamagedSignature, float, newHealth, float, maxHealth, float, changeInHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPawnStaminaChangedSignature, float, currentStamina, float, maxStamina, float, changedStamina);


UCLASS()
class CFORENGINES_API AP_FPS : public ACharacter, public IInputable
{
	GENERATED_BODY()

public:
	AP_FPS();

	virtual void Input_Move_Implementation(FVector2D value) override;
	virtual void Input_Look_Implementation(FVector2D value) override;
	virtual void Input_JumpPressed_Implementation() override;
	virtual void Input_JumpReleased_Implementation() override;
	virtual void Input_FirePressed_Implementation() override;
	virtual void Input_FireReleased_Implementation() override;
	virtual void Input_SprintPressed_Implementation() override;
	virtual void Input_SprintReleased_Implementation() override;
	virtual void Input_CrouchPressed_Implementation() override;
	virtual void Input_CrouchReleased_Implementation() override;
	virtual void Input_Interact_Implementation() override;

	virtual UInputMappingContext* GetMappingContext_Implementation() override;

	virtual UBehaviorTree* GetBehaviorTree_Implementation() override;

	UPROPERTY(BlueprintAssignable)
	FPawnDamagedSignature OnPawnDamaged;
	FPawnStaminaChangedSignature OnPawnStaminaChanged;
	

protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> _Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UHealthComponent> _Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaminaComponent> _Stamina2;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> _WeaponAttachPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon_Base> _DefaultWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AWeapon_Base> _WeaponRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInputMappingContext> _MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBehaviorTree> _BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCharacterMovementComponent> _MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _SprintMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _NormalMoveSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector _CrouchEyeOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float _CrouchSpeed;

	FTimerHandle _CrouchTimer;

	void BeginPlay() override;
	/*void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;
	void LerpCrouchCamera();*/

private:
	UFUNCTION()
	void Handle_HealthDead(AController* causer);
	UFUNCTION()
	void Handle_HealthDamaged(float currentHealth, float maxHealth, float changedHealth);

	UFUNCTION()
	void Handle_StoppedSprinting();
	UFUNCTION()
	void Handle_ChangeStamina(float currentStamina, float maxStamina, float changedStamina);

	AActor* GetClosest(TArray<FHitResult> hit);
};
