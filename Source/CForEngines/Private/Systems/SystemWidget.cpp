#include "Systems/SystemWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USystemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(PowerBar) { PowerBar->SetPercent(1.f); }
	if(OxygenBar) { OxygenBar->SetPercent(1.f); }

	PowerAlarm->SetBrushTintColor(_AlarmNormalColour);
	OxygenAlarm->SetBrushTintColor(_AlarmNormalColour);
}

void USystemWidget::UpdatePower(float newPowerRatio)
{
	if(!PowerBar) { return; }
	
	if(!IsShowingPowerAlarm)
	{
		float currentPowerRatio = PowerBar->GetPercent();
		if(newPowerRatio < currentPowerRatio && !IsShowingPowerAlarm) { Handle_PowerAlarm(); }
	}
	
	PowerBar->SetPercent(newPowerRatio);
}

void USystemWidget::UpdateOxygen(float newOxygenRatio)
{
	if(!OxygenBar) { return; }

	if(!IsShowingOxygenAlarm)
	{
		float currentOxygenRatio = OxygenBar->GetPercent();
		if(newOxygenRatio < currentOxygenRatio && !IsShowingOxygenAlarm) { Handle_OxygenAlarm(); }
	}
	
	OxygenBar->SetPercent(newOxygenRatio);
}

void USystemWidget::Handle_PowerAlarm()
{
	if(!IsShowingPowerAlarm) { IsShowingPowerAlarm = true; }

	if(PowerAlarm->GetBrush().TintColor == _AlarmNormalColour)
	{
		PowerAlarm->SetBrushTintColor(_AlarmLitColour);
		if(!_IsPlayingAlarmSound) { OnPlayAlarmSound.Broadcast(); _IsPlayingAlarmSound = true; }
	}
	else { PowerAlarm->SetBrushTintColor(_AlarmNormalColour); }

	if(!GetWorld()->GetTimerManager().IsTimerActive(_PowerAlarmTimer))
		{ GetWorld()->GetTimerManager().SetTimer(_PowerAlarmTimer, this, &USystemWidget::Handle_PowerAlarm, _PowerAlarmTime, true); }
}

void USystemWidget::StopPowerAlarm()
{
	if(GetWorld()->GetTimerManager().IsTimerActive(_PowerAlarmTimer)) { GetWorld()->GetTimerManager().ClearTimer(_PowerAlarmTimer); }
	IsShowingPowerAlarm = false;
	PowerAlarm->SetBrushTintColor(_AlarmNormalColour);

	if(!GetWorld()->GetTimerManager().IsTimerActive(_OxygenAlarmTimer)) { OnStopAlarmSound.Broadcast(); _IsPlayingAlarmSound = false; }
}



void USystemWidget::Handle_OxygenAlarm()
{
	if(!IsShowingOxygenAlarm) { IsShowingOxygenAlarm = true; }

	if(OxygenAlarm->GetBrush().TintColor == _AlarmNormalColour)
	{
		OxygenAlarm->SetBrushTintColor(_AlarmLitColour);
		if(!_IsPlayingAlarmSound) { OnPlayAlarmSound.Broadcast(); _IsPlayingAlarmSound = true; }
	}
	else { OxygenAlarm->SetBrushTintColor(_AlarmNormalColour); }

	if(!GetWorld()->GetTimerManager().IsTimerActive(_OxygenAlarmTimer))
	{ GetWorld()->GetTimerManager().SetTimer(_OxygenAlarmTimer, this, &USystemWidget::Handle_OxygenAlarm, _OxygenAlarmTime, true); }
}

void USystemWidget::StopOxygenAlarm()
{
	if(GetWorld()->GetTimerManager().IsTimerActive(_OxygenAlarmTimer)) { GetWorld()->GetTimerManager().ClearTimer(_OxygenAlarmTimer); }
	IsShowingOxygenAlarm = false;
	OxygenAlarm->SetBrushTintColor(_AlarmNormalColour);

	if(!GetWorld()->GetTimerManager().IsTimerActive(_PowerAlarmTimer)) { OnStopAlarmSound.Broadcast(); _IsPlayingAlarmSound = false; }
}

void USystemWidget::UpdateClock(int hours, int minutes)
{
	if(Clock)
	{
		FString clockText;
		if(hours == 0) { clockText = TEXT("12"); }
		else { clockText = FString::Printf(TEXT("%d"), hours); }

		if(minutes < 10) { clockText += FString::Printf(TEXT(" : 0%d"), minutes); }
		else { clockText += FString::Printf(TEXT(" : %d"), minutes); }
		
		Clock->SetText(
			FText::FromString(
				FString::Printf(TEXT("%s"), *clockText)
			)
		);
	}
}