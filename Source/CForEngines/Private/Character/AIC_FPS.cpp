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
		AActor* spawnActor = GameMode->FindPlayerStart_Implementation(this, "Enemy");

		UWorld* world = GetWorld();
		if(world == nullptr || _EnemyPawn == nullptr) { return; }
		
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = nullptr;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		AActor* enemyActor = world->SpawnActor(_EnemyPawn, &spawnActor->GetActorTransform(), spawnParams);
		OnPossess(Cast<APawn>(enemyActor));
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

