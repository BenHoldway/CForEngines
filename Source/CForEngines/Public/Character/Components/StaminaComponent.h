#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStoppedSprintingSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FStaminaValueChangedSignature, float, newStamina, float, maxStamina, float, changedStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStoppedStaminaRegainSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CFORENGINES_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStaminaComponent();

	UFUNCTION()
	void StartSprinting();
	UFUNCTION()
	void Sprint();
	UFUNCTION()
	void StopSprinting(float delay);
	
	FStoppedSprintingSignature OnStoppedSprinting;
	FStaminaValueChangedSignature OnStaminaValueChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _MaxStamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float _CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _StaminaChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _StaminaDrainInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _StaminaRegainInterval;

	FTimerManager* _TimerManager;
	FTimerHandle _SprintTimer;
	FTimerHandle _SprintRegainTimer;
	
	virtual void BeginPlay() override;
	void BroadcastStaminaRegain();
	UFUNCTION()
	void RegainStamina();

};
