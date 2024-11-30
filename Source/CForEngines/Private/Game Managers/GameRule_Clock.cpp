#include "Game Managers/GameRule_Clock.h"

UGameRule_Clock::UGameRule_Clock()
{
	PrimaryComponentTick.bCanEverTick = false;

	_SecondsForEachInGameMinute = 1;
	_ClockMinuteIncrement = 5;
	_CurrentHours = 0;
	_CurrentMinutes = 0;
}

void UGameRule_Clock::Init()
{
	Super::Init();

	//timer time
	//in game clock time: 12am - 6am
	//hour (timer seconds): timer time / 6
	//minute (timer seconds) = hour / 60
	
	_TimerManager = &GetWorld()->GetTimerManager();

	//time for each minute * 60 (total time for 1 hour) * 6 (for the 6 hours)
	float totalTime = _SecondsForEachInGameMinute * 60 * 6;
	_TimerManager->SetTimer(_ClockTimer, this, &UGameRule_Clock::Handle_ClockCompleted, totalTime);
	_TimerManager->SetTimer(_UpdateClockTimer, this, &UGameRule_Clock::Handle_UpdateClock, _SecondsForEachInGameMinute * _ClockMinuteIncrement);
}

void UGameRule_Clock::Handle_UpdateClock()
{
	_CurrentMinutes += _ClockMinuteIncrement;
	if(_CurrentMinutes >= 60)
	{
		_CurrentMinutes -= 60;
		_CurrentHours += 1;
	}
	OnUpdateClock.Broadcast(_CurrentHours, _CurrentMinutes);
}

void UGameRule_Clock::Handle_ClockCompleted()
{
	BroadcastComplete(true);
}

