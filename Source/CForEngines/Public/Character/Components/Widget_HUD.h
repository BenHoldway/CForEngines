#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_HUD.generated.h"

class UBorder;
class UTextBlock;
class UProgressBar;

UCLASS(Abstract, BlueprintType)
class CFORENGINES_API UWidget_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void UpdateHealth(float newHealthRatio);
	void UpdateScore(int newScore);
	void StartStaminaChange(float newStamina);
	void ShowInteractPrompt();
	void HideInteractPrompt();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> InteractPrompt;

	

	UFUNCTION()
	void FadeOutStamina();

	FTimerManager* _TimerManager;
	FTimerHandle _HideStaminaTimer;
	FTimerHandle _ChangeStaminaTimer;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float _HideStaminaTimeInterval;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float _HideStaminaTimeDelay;

	float _NewStamina;
};
