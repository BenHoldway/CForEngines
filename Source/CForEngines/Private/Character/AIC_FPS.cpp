#include "Character/AIC_FPS.h"

#include "Character/Components/Inputable.h"
#include "Kismet/KismetSystemLibrary.h"

AAIC_FPS::AAIC_FPS()
{
	PrimaryActorTick.bCanEverTick = true;
}

ETeamAttitude::Type AAIC_FPS::GetTeamAttitudeTowards(const AActor& Other) const
{
	return Super::GetTeamAttitudeTowards(Other);
}

void AAIC_FPS::BeginPlay()
{
	Super::BeginPlay();
}

void AAIC_FPS::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(UKismetSystemLibrary::DoesImplementInterface(InPawn, UInputable::StaticClass()))
	{
		RunBehaviorTree(IInputable::Execute_GetBehaviorTree(InPawn));
	}
}

void AAIC_FPS::Handle_PerceptionUpdated()
{
}

