#include "Game Managers/GM_Widget.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"

void UGM_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	if(WinScreen) { WinScreen->SetVisibility(ESlateVisibility::Hidden); }
	if(LoseScreen) { LoseScreen->SetVisibility(ESlateVisibility::Hidden); }

	if(WinScreenReplayButton) { WinScreenReplayButton->OnClicked.AddUniqueDynamic(this, &UGM_Widget::ReplayGame); }
	if(LoseScreenReplayButton) { LoseScreenReplayButton->OnClicked.AddUniqueDynamic(this, &UGM_Widget::ReplayGame); }
}

void UGM_Widget::ShowWinScreen()
{
	if(LoseScreen) { if(LoseScreen->GetVisibility() == ESlateVisibility::Visible) { return; } }
	if(WinScreen) { WinScreen->SetVisibility(ESlateVisibility::Visible); }
}

void UGM_Widget::ShowLoseScreen()
{
	if(WinScreen) { if(WinScreen->GetVisibility() == ESlateVisibility::Visible) { return; } }
	if(LoseScreen) { LoseScreen->SetVisibility(ESlateVisibility::Visible); }
}

void UGM_Widget::ReplayGame()
{
	OnReplay.Broadcast();
}
