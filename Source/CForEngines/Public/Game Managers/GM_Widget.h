#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GM_Widget.generated.h"

class UButton;
class UCanvasPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReplayGameSignature);

UCLASS(Abstract, BlueprintType)
class CFORENGINES_API UGM_Widget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void ShowWinScreen();
	void ShowLoseScreen();

	UFUNCTION()
	void ReplayGame();

	FReplayGameSignature OnReplay;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> WinScreen;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> LoseScreen;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> WinScreenReplayButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LoseScreenReplayButton;
};
