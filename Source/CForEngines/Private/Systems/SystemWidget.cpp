// Fill out your copyright notice in the Description page of Project Settings.


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
	if(PowerBar) { PowerBar->SetPercent(newPowerRatio); UE_LOG(LogTemp, Display, TEXT("Power Updated")); }
}

void USystemWidget::UpdateOxygen_Implementation(float newOxygenRatio)
{
	if(OxygenBar) { OxygenBar->SetPercent(newOxygenRatio); UE_LOG(LogTemp, Display, TEXT("Oxygen Updated")); }
}
