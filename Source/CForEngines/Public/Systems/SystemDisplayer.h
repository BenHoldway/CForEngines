#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SystemDisplayer.generated.h"

class USystemWidget;
class UWidgetComponent;
class UBoxComponent;
class UStaticMeshComponent;
class ASystemDisplayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSystemDisplayRegisteredSignature, ASystemDisplayer*, systemDisplay);

UCLASS(Abstract)
class CFORENGINES_API ASystemDisplayer : public AActor
{
	GENERATED_BODY()

public:
	ASystemDisplayer();

	static inline FSystemDisplayRegisteredSignature OnRegister;

	void UpdatePower(float max, float current);
	void UpdateOxygen(float max, float current);
	void StopPowerAlarm();
	void StopOxygenAlarm();
	void UpdateClock(int hours, int minutes);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void PlayAlarmSound();
	UFUNCTION()
	void StopAlarmSound();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> _Collider;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> _SystemWidgetComponent;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<USystemWidget> _SystemWidgetClass;
	TObjectPtr<USystemWidget> _SystemWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> _AlarmSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _AlarmSoundWaitTime;
	UPROPERTY()
	TObjectPtr<UAudioComponent> _AlarmSoundComponent;

	FTimerHandle _AlarmTimer;

public:
};
