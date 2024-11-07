#pragma once

#include "CoreMinimal.h"
#include "Systemable.h"
#include "Blueprint/UserWidget.h"
#include "SystemWidget.generated.h"

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
	TObjectPtr<UProgressBar> OxygenBar;
};
