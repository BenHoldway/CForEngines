#include "Systems/SystemWidget.h"

#include "Components/ProgressBar.h"

void USystemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(PowerBar) { PowerBar->SetPercent(1.f); }
	if(OxygenBar) { OxygenBar->SetPercent(1.f); }
}

void USystemWidget::UpdatePower_Implementation(float newPowerRatio)
{
	if(PowerBar)
	{
		if(!IsShowingPowerAlarm)
		{
			float currentPowerRatio = PowerBar->GetPercent();
			if(newPowerRatio < currentPowerRatio) { IsShowingPowerAlarm = true; }
		}
		
		PowerBar->SetPercent(newPowerRatio);
	}
}

void USystemWidget::UpdateOxygen_Implementation(float newOxygenRatio)
{
	if(OxygenBar)
	{
		if(!IsShowingPowerAlarm)
		{
			float currentOxygenRatio = PowerBar->GetPercent();
			if(newOxygenRatio < currentOxygenRatio) { IsShowingPowerAlarm = true; }
		}
		
		OxygenBar->SetPercent(newOxygenRatio);
	}
}

void USystemWidget::StartPowerAlarm()
{
	
}

void USystemWidget::StartOxygenAlarm()
{
	
}
