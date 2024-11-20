#include "Character/AIC_FPS.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Components/Inputable.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Game Managers/GM_FPS.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAIC_FPS::AAIC_FPS()
{
	PrimaryActorTick.bCanEverTick = false;

	_AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));

	_AISense_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	_AISense_Sight->DetectionByAffiliation.bDetectEnemies = true;
	_AISense_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	_AISense_Sight->DetectionByAffiliation.bDetectNeutrals = false;

	_AIPerception->ConfigureSense(*_AISense_Sight);
	_AIPerception->SetDominantSense(UAISenseConfig_Sight::StaticClass());

	AAIController::SetGenericTeamId(FGenericTeamId(1));
}

ETeamAttitude::Type AAIC_FPS::GetTeamAttitudeTowards(const AActor& Other) const
{
	FGenericTeamId TeamId(FGenericTeamId::GetTeamIdentifier(Other.GetInstigatorController()));
	
	if(TeamId == FGenericTeamId(1))
	{
		return ETeamAttitude::Friendly;
	}
	else if (TeamId == FGenericTeamId(2))
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Neutral;
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

	_Blackboard = GetBlackboardComponent();

	_AIPerception->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AAIC_FPS::Handle_TargetPerceptionUpdated);
	_AIPerception->OnTargetPerceptionForgotten.AddUniqueDynamic(this, &AAIC_FPS::Handle_TargetPerceptionForgotten);

	_AISense_Sight->SightRadius = _Blackboard->GetValueAsFloat("EngagingDistance");
	_AISense_Sight->LoseSightRadius = _Blackboard->GetValueAsFloat("EngagingDistance");

	FEnvQueryRequest EQR_FindWanderPos = FEnvQueryRequest(_EQS_FindWanderPos, GetPawn());
	EQR_FindWanderPos.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &AAIC_FPS::Handle_FindWanderPosResult);
}

void AAIC_FPS::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(UKismetSystemLibrary::DoesImplementInterface(InPawn, UInputable::StaticClass()))
	{
		RunBehaviorTree(IInputable::Execute_GetBehaviorTree(InPawn));
	}
}

void AAIC_FPS::Handle_TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	switch(Stimulus.Type)
	{
		case 0:
			if(_Blackboard->GetValueAsObject("TargetActor") == Actor) { _Blackboard->ClearValue("TargetActor"); }
			else { _Blackboard->SetValueAsObject("TargetActor", Actor); }
			return;
		default:
			return;
	}
}

void AAIC_FPS::Handle_TargetPerceptionForgotten(AActor* Actor)
{
	UE_LOG(LogTemp, Display, TEXT("Target Forgotten"));
	_Blackboard->ClearValue("TargetActor");
}

void AAIC_FPS::Handle_FindWanderPosResult(TSharedPtr<FEnvQueryResult> result)
{
	if(result->IsSuccessful())
	{
		_Blackboard->SetValueAsVector("WanderPos", result->GetItemAsLocation(0));
	}
}

