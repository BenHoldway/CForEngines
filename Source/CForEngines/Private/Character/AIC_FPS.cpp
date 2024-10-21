#include "Character/AIC_FPS.h"

#include "Character/Components/Inputable.h"
#include "Kismet/KismetSystemLibrary.h"

AAIC_FPS::AAIC_FPS()
{
	PrimaryActorTick.bCanEverTick = true;
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

