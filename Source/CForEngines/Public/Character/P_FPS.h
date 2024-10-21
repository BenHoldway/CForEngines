#pragma once

#include "CoreMinimal.h"
#include "Components/Inputable.h"
#include "GameFramework/Character.h"
#include "P_FPS.generated.h"

class AWeapon_Base;
class UHealthComponent;
class UCharacterMovementComponent;
class UCameraComponent;
class UCapsuleComponent;
class UBehaviorTree;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPawnDamagedSignature, float, newHealth, float, maxHealth, float, changeInHealth);

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

	virtual UInputMappingContext* GetMappingContext_Implementation() override;

	virtual UBehaviorTree* GetBehaviorTree_Implementation() override;

	UPROPERTY(BlueprintAssignable)
	FPawnDamagedSignature OnPawnDamaged;

protected:
	void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> _Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UHealthComponent> _Health;

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

private:
	UFUNCTION()
	void Handle_HealthDead(AController* causer);
	UFUNCTION()
	void Handle_HealthDamaged(float currentHealth, float maxHealth, float changedHealth);
};
