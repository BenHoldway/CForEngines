#include "Game Managers/GM_Widget.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Kismet/KismetSystemLibrary.h"

void UGM_Widget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ShowMainMenu();
	if(PauseMenu) { PauseMenu->SetVisibility(ESlateVisibility::Hidden); }
	if(WinScreen) { WinScreen->SetVisibility(ESlateVisibility::Hidden); }
	if(LoseScreen) { LoseScreen->SetVisibility(ESlateVisibility::Hidden); }

	if(WinScreenReplayButton) { WinScreenReplayButton->OnClicked.AddUniqueDynamic(this, &UGM_Widget::ReplayGame); }
	if(LoseScreenReplayButton) { LoseScreenReplayButton->OnClicked.AddUniqueDynamic(this, &UGM_Widget::ReplayGame); }
	if(PlayGameButton) { PlayGameButton->OnClicked.AddUniqueDynamic(this, &UGM_Widget::PlayGame); }
	if(ExitGameButton) { ExitGameButton->OnClicked.AddUniqueDynamic(this, &UGM_Widget::ExitGame); }
	if(MainMenuButton) { MainMenuButton->OnClicked.AddUniqueDynamic(this, &UGM_Widget::ShowMainMenu); }
}

void UGM_Widget::ShowMainMenu()
{
	if(MainMenu) { MainMenu->SetVisibility(ESlateVisibility::Visible); }
}

void UGM_Widget::HideMainMenu()
{
	if(MainMenu) { MainMenu->SetVisibility(ESlateVisibility::Hidden); }
}

void UGM_Widget::ShowPauseMenu()
{
	if(PauseMenu) { PauseMenu->SetVisibility(ESlateVisibility::Visible); }
}

void UGM_Widget::HidePauseMenu()
{
	if(PauseMenu) { PauseMenu->SetVisibility(ESlateVisibility::Hidden); OnUnpause.Broadcast(); }
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

void UGM_Widget::PlayGame()
{
	OnPlay.Broadcast();
	HideMainMenu();
}

void UGM_Widget::ReplayGame()
{
	OnReplay.Broadcast();
}

void UGM_Widget::ExitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
