#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "System.generated.h"

class ASystem_Controller;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FValueChangedSignature,
                                             float, max, float, current);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDepletedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFaultStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFaultStoppedSignature);

UENUM()
enum ESystemType
{
	Power = 0 UMETA(DisplayName = "Power"),
	Oxygen = 1 UMETA(DisplayName = "Oxygen")
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CFORENGINES_API USystem : public UActorComponent
{
	GENERATED_BODY()

public:
	USystem();

	void Init(ASystem_Controller* owner);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESystemType> _SystemType;

	UPROPERTY(BlueprintAssignable)
	FDepletedSignature OnDepleted;
	UPROPERTY(BlueprintAssignable)
	FValueChangedSignature OnValueChanged;

	UPROPERTY(BlueprintAssignable)
	FFaultStartedSignature OnFaultStarted;
	UPROPERTY(BlueprintAssignable)
	FFaultStoppedSignature OnFaultStopped;

protected:
	UPROPERTY()
	ASystem_Controller* _Controller;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _MaxValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float _CurrentValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _MinFaultTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _MaxFaultTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _ValueChangeAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _ValueChangeTime;

	bool _FaultHasStarted;

	FTimerManager* _TimerManager;
	FTimerHandle _StartFaultTimer;
	FTimerHandle _ValueChangeTimer;
	
	virtual void BeginPlay() override;

public:
	void SetFaultTimer();
	
	void Handle_FaultStarted();
	void Handle_FaultStopped();
	
	void Handle_ValueChanged();
	void Handle_Regenerated();

	bool HasFaultStarted();
};
