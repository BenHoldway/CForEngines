#include "Game Managers/GameRule_Clock.h"

UGameRule_Clock::UGameRule_Clock()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGameRule_Clock::Init()
{
	Super::Init();


	_TimerManager = &GetWorld()->GetTimerManager();
	_TimerManager->SetTimer(_ClockTimer, this, &UGameRule_Clock::Handle_ClockCompleted, _Time);
	UE_LOG(LogTemp, Display, TEXT("Clock Started"));
}

void UGameRule_Clock::Handle_ClockCompleted()
{
	BroadcastComplete(true);
}

