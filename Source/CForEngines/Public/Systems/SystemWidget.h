#pragma once

#include "CoreMinimal.h"
#include "Systemable.h"
#include "Blueprint/UserWidget.h"
#include "SystemWidget.generated.h"

class UTextBlock;
class UImage;
class UProgressBar;

UCLASS(Abstract, BlueprintType)
class CFORENGINES_API USystemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void UpdatePower(float newPowerRatio);
	void UpdateOxygen(float newOxygenRatio);

	void StopPowerAlarm();
	void StopOxygenAlarm();

	void UpdateClock(int hours, int minutes);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PowerBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PowerAlarm;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> OxygenBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> OxygenAlarm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor _AlarmNormalColour;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor _AlarmLitColour;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Clock;

	bool IsShowingPowerAlarm;
	bool IsShowingOxygenAlarm;

	void Handle_PowerAlarm();
	void Handle_OxygenAlarm();

	FTimerHandle _PowerAlarmTimer;
	FTimerHandle _OxygenAlarmTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _PowerAlarmTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _OxygenAlarmTime;
};
