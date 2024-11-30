#pragma once

#include "CoreMinimal.h"
#include "GameRule.h"
#include "GameRule_Clock.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateClockSignature, int, hours, int, minutes);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CFORENGINES_API UGameRule_Clock : public UGameRule
{
	GENERATED_BODY()

public:
	UGameRule_Clock();

	virtual void Init() override;

	UPROPERTY(BlueprintAssignable)
	FUpdateClockSignature OnUpdateClock;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _SecondsForEachInGameMinute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _ClockMinuteIncrement;

	int _CurrentMinutes;
	int _CurrentHours;
	
	FTimerManager* _TimerManager;
	FTimerHandle _ClockTimer;
	FTimerHandle _UpdateClockTimer;

	void Handle_UpdateClock();
	void Handle_ClockCompleted();

public:
};
