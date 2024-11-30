#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GM_Widget.generated.h"

class UButton;
class UCanvasPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReplayGameSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnpauseGameSignature);

UCLASS(Abstract, BlueprintType)
class CFORENGINES_API UGM_Widget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void ShowMainMenu();
	void HideMainMenu();

	void ShowPauseMenu();
	void HidePauseMenu();
	
	void ShowWinScreen();
	void ShowLoseScreen();

	UFUNCTION()
	void PlayGame();
	UFUNCTION()
	void ExitGame();

	FReplayGameSignature OnReplay;
	FUnpauseGameSignature OnUnpause;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> MainMenu;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> PauseMenu;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> WinScreen;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> LoseScreen;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> WinScreenReplayButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LoseScreenReplayButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PlayGameButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitGameButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MainMenuButton;
};
