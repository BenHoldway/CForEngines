#include "Game Managers/GM_FPS.h"

#include "Character/AIC_FPS.h"
#include "Character/CustomPawnStart.h"
#include "Character/Components/Controllable.h"
#include "Game Managers/GameRule.h"
#include "Game Managers/GameRule_Systems.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/System.h"

void AGM_FPS::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	if(world == nullptr || _EnemyControllerClass == nullptr) { return; }

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = GetOwner();
	spawnParams.Instigator = GetInstigator();
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	FVector location(0, 0, 0);
	FRotator rotation = FRotator::ZeroRotator;
	
	world->SpawnActor(_EnemyControllerClass, &location, &rotation, spawnParams);
}

AActor* AGM_FPS::FindPlayerStart_Implementation(AController* Character, const FString& IncomingName)
{
	if(_PlayerStarts.Num() == 0 || _EnemyStarts.Num() == 0)
	{
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), foundActors);
		
		for(AActor* actor : foundActors)
		{
			if(ACustomPawnStart* pawnStart = Cast<ACustomPawnStart>(actor)) { _EnemyStarts.Add(actor); }
			else { _PlayerStarts.Add(actor); }
		}
	}
	
	if(_PlayerStarts.Num() > 0 && Character->IsA(APlayerController::StaticClass()))
	{
		AActor* start = _PlayerStarts[FMath::RandRange(0, _PlayerStarts.Num()-1)];
		_PlayerStarts.Remove(start);
		_UsedStarts.Add(start);
		return start;
	}
	if(_EnemyStarts.Num() > 0 && Character->IsA(AAIController::StaticClass()))
	{
		AActor* start = _EnemyStarts[FMath::RandRange(0, _EnemyStarts.Num()-1)];
		_EnemyStarts.Remove(start);
		_UsedStarts.Add(start);
		return start;
	}
	
	return nullptr;
}
 
void AGM_FPS::PostLogin(APlayerController* NewPlayer)
{
	_PlayerControllers.AddUnique(NewPlayer);
	Super::PostLogin(NewPlayer);
}
 
void AGM_FPS::Logout(AController* Exiting)
{
	_PlayerControllers.Remove(Exiting);
	Super::Logout(Exiting);
}

void AGM_FPS::HandleMatchIsWaitingToStart()
{
	TArray<UActorComponent*> outComponents;
	GetComponents(outComponents);
	for(UActorComponent* comp: outComponents)
	{
		if(UGameRule* rule = Cast<UGameRule>(comp))
		{
			_GameRuleManagers.Add(rule);
			rule->Init();
			rule->OnComplete.AddUniqueDynamic(this, &AGM_FPS::Handle_GameRuleCompleted);
			rule->OnPointsScored.AddUniqueDynamic(this, &AGM_FPS::Handle_GameRulePointsScored);
			_GameRulesLeft++;

			if(UGameRule_Systems* systemRule = Cast<UGameRule_Systems>(rule))
			{
				systemRule->OnSystemDepleted.AddUniqueDynamic(this, &AGM_FPS::Handle_GameRuleSystemDepleted);
			}
		}
	}
	
	StartMatch();
	Super::HandleMatchIsWaitingToStart();
}
 
void AGM_FPS::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}
 
void AGM_FPS::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}
 
void AGM_FPS::OnMatchStateSet()
{
	FString output;
	if(MatchState == MatchState::WaitingToStart)
	{
		output = "Waiting To Start";
	}
	else if(MatchState == MatchState::InProgress)
	{
		output = "InProgress";
	}
	else if(MatchState == MatchState::WaitingPostMatch)
	{
		output = "Waiting Post Match";
	}
	else if(MatchState == MatchState::LeavingMap)
	{
		output = "Leaving Map";
	}
 
	UE_LOG(LogTemp, Display, TEXT("MATCH STATE CHANGED: %s"), *output);
    
	Super::OnMatchStateSet();
}
 
bool AGM_FPS::ReadyToStartMatch_Implementation() { return false; }
 
bool AGM_FPS::ReadyToEndMatch_Implementation() { return false; }

void AGM_FPS::Handle_GameRulePointsScored(AController* scorer, int points)
{
	if(UKismetSystemLibrary::DoesImplementInterface(scorer, UControllable::StaticClass()))
	{
		IControllable::Execute_AddPoints(scorer, points);
	}
}

void AGM_FPS::Handle_GameRuleCompleted(bool successful)
{
	if(successful) { UE_LOG(LogTemp, Display, TEXT("Success")); }
	else { UE_LOG(LogTemp, Display, TEXT("Failure")); }
}

void AGM_FPS::Handle_GameRuleSystemDepleted(ESystemType systemType)
{
	switch (systemType)
	{
		case ESystemType::Power:
			UE_LOG(LogTemp, Display, TEXT("Power Depleted"))
		case ESystemType::Oxygen:
			UE_LOG(LogTemp, Display, TEXT("Oxygen Depleted"))
		default:
			break;
	}
}
