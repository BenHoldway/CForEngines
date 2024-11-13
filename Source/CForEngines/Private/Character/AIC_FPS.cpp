#include "Character/AIC_FPS.h"

#include "Character/Components/Inputable.h"
#include "Game Managers/GM_FPS.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AAIC_FPS::AAIC_FPS()
{
	PrimaryActorTick.bCanEverTick = false;
}

ETeamAttitude::Type AAIC_FPS::GetTeamAttitudeTowards(const AActor& Other) const
{
	return Super::GetTeamAttitudeTowards(Other);
}

void AAIC_FPS::BeginPlay()
{
	Super::BeginPlay();

	if(AGM_FPS* GameMode = Cast<AGM_FPS>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->FindPlayerStart_Implementation(this, "Enemy");
	}
}

void AAIC_FPS::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UE_LOG(LogTemp, Display, TEXT("AI Spawned"));

	if(UKismetSystemLibrary::DoesImplementInterface(InPawn, UInputable::StaticClass()))
	{
		RunBehaviorTree(IInputable::Execute_GetBehaviorTree(InPawn));
	}
}

void AAIC_FPS::Handle_PerceptionUpdated()
{
}

