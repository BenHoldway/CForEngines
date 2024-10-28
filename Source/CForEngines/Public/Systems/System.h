#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "System.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FValueChangedSignature,
	float, max, float, current);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDepletedSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CFORENGINES_API USystem : public UActorComponent
{
	GENERATED_BODY()

public:
	USystem();

	UPROPERTY(BlueprintAssignable)
	FDepletedSignature OnDepleted;
	UPROPERTY(BlueprintAssignable)
	FValueChangedSignature OnValueChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _MaxValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float _CurrentValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _ValueChangeAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _ValueChangeTime;

	FTimerManager* _TimerManager;
	FTimerHandle _ValueChangeTimer;
	
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void SetTimer();
	UFUNCTION()
	void Handle_ValueChanged();
	UFUNCTION()
	void Handle_Regenerated();
};
