#pragma once

#include "CoreMinimal.h"
#include "Systemable.h"
#include "Blueprint/UserWidget.h"
#include "SystemWidget.generated.h"

class UTextBlock;
class UImage;
class UProgressBar;

UCLASS(Abstract, BlueprintType)
class CFORENGINES_API USystemWidget : public UUserWidget, public ISystemable
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void UpdatePower_Implementation(float newPowerRatio) override;
	virtual void UpdateOxygen_Implementation(float newOxygenRatio) override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PowerBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PowerAlarm;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> OxygenBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> OxygenAlarm;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Clock;

	bool IsShowingPowerAlarm;
	bool IsShowingOxygenAlarm;

	void StartPowerAlarm();
	void StartOxygenAlarm();

	FTimerHandle _PowerAlarmTimer;
	FTimerHandle _OxygenAlarmTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _PowerAlarmTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _OxygenAlarmTime;
};
