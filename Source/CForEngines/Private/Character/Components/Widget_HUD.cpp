#include "Character/Components/Widget_HUD.h"

#include "Components/Border.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UWidget_HUD::NativeConstruct()
{
	Super::NativeConstruct();

	_TimerManager = &GetWorld()->GetTimerManager();

	if(HealthBar) { HealthBar->SetPercent(1.0f); }
	if(ScoreText) { ScoreText->SetText(FText::FromString("Score: 0")); }
	if(StaminaBar)
	{
		StaminaBar->SetPercent(1.0f);
		StaminaBar->SetVisibility(ESlateVisibility::Hidden);
	}

	if(InteractPrompt) { InteractPrompt->SetVisibility(ESlateVisibility::Hidden); }
}

void UWidget_HUD::UpdateHealth(float newHealthRatio)
{
	if(HealthBar) { HealthBar->SetPercent(newHealthRatio); }
}

void UWidget_HUD::UpdateScore(int newScore)
{
	if(ScoreText)
	{
		ScoreText->SetText(
			FText::FromString(
				FString::Printf(TEXT("Score: %d"), newScore)
			)
		);
	}
}

void UWidget_HUD::StartStaminaChange(float newStamina)
{
	if(StaminaBar)
	{
		if(!StaminaBar->IsVisible() || StaminaBar->GetRenderOpacity() == 0.0f)
		{
			StaminaBar->SetRenderOpacity(1.0f);
			StaminaBar->SetVisibility(ESlateVisibility::Visible);
		}

		_NewStamina = newStamina;

		StaminaBar->SetPercent(_NewStamina);

		if(StaminaBar->GetPercent() == 1.0f)
		{
			_TimerManager->SetTimer(_HideStaminaTimer, this, &UWidget_HUD::FadeOutStamina, _HideStaminaTimeInterval, true, _HideStaminaTimeDelay);
		}
	}
}

void UWidget_HUD::ShowInteractPrompt()
{
	InteractPrompt->SetVisibility(ESlateVisibility::Visible);
}

void UWidget_HUD::HideInteractPrompt()
{
	InteractPrompt->SetVisibility(ESlateVisibility::Hidden);
}

void UWidget_HUD::FadeOutStamina()
{
	if(StaminaBar)
	{
		StaminaBar->SetRenderOpacity(StaminaBar->GetRenderOpacity() - 0.1f);
		if(StaminaBar->GetRenderOpacity() <= 0.0f)
		{
			StaminaBar->SetRenderOpacity(0.0f);
			_TimerManager->ClearTimer(_HideStaminaTimer);
		}
	}
}
