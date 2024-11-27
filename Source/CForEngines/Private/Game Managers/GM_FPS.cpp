#include "Game Managers/GM_FPS.h"

#include "Character/AIC_FPS.h"
#include "Character/CustomPawnStart.h"
#include "Character/PC_FPS.h"
#include "Character/Components/Controllable.h"
#include "Game Managers/GameRule.h"
#include "Game Managers/GameRule_Systems.h"
#include "Game Managers/GM_Widget.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/System.h"

void AGM_FPS::BeginPlay()
{
	Super::BeginPlay();

	_PlayerSpawnIndex = 0;
	_EnemySpawnIndex = 0;

	UWorld* world = GetWorld();
	if(world == nullptr || _EnemyControllerClass == nullptr) { return; }

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = GetOwner();
	spawnParams.Instigator = GetInstigator();
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	FVector location(0, 0, 0);
	FRotator rotation = FRotator::ZeroRotator;
	
	world->SpawnActor(_EnemyControllerClass, &location, &rotation, spawnParams);

	if(_GMWidgetClass)
	{
		_GMWidget = CreateWidget<UGM_Widget, UWorld*>(GetWorld(), _GMWidgetClass);
		_GMWidget->AddToViewport();
		_GMWidget->OnReplay.AddUniqueDynamic(this, &AGM_FPS::ReplayGame);
	}
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
			else
			{
				_PlayerStarts.Add(actor);
			}
		}
	}

	if(_EnemyStarts.Num() > 0 && Character->IsA(AAIController::StaticClass()))
	{
		if(_EnemyControllers.Contains(Character)) { return _EnemyControllers[_EnemySpawnIndex - 1]; }
		AActor* start = _EnemyStarts[_EnemySpawnIndex];
		_EnemyControllers.AddUnique(Character);
		_EnemySpawnIndex++;
		return start;
	}
	if(_PlayerStarts.Num() > 0 && Character->IsA(APC_FPS::StaticClass()))
	{
		if(_PlayerControllers.Contains(Character)) { return _PlayerStarts[_PlayerSpawnIndex - 1]; }
		AActor* start = _PlayerStarts[_PlayerSpawnIndex];
		_PlayerControllers.AddUnique(Character);
		_PlayerSpawnIndex++;

		if(APC_FPS* playerController = Cast<APC_FPS>(Character))
		{
			playerController->OnPlayerDead.AddUniqueDynamic(this, &AGM_FPS::PlayerDead);
			playerController->OnPlayerDamaged.AddUniqueDynamic(this, &AGM_FPS::ResetEntities);
		}
		
		return start;
	}
	
	return nullptr;
}
 
void AGM_FPS::PostLogin(APlayerController* NewPlayer)
{
	//_PlayerControllers.AddUnique(NewPlayer);
	Super::PostLogin(NewPlayer);
}
 
void AGM_FPS::Logout(AController* Exiting)
{
	//_PlayerControllers.Remove(Exiting);
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

void AGM_FPS::ResetEntities()
{
	for(int playerController = 0; playerController < _PlayerControllers.Num(); playerController++)
	{
		if(UKismetSystemLibrary::DoesImplementInterface(_PlayerControllers[playerController], UResetable::StaticClass()))
		{
			IResetable::Execute_Reset(_PlayerControllers[playerController], _PlayerStarts[playerController]->GetActorLocation());
		}
	}

	for(int enemyController = 0; enemyController < _EnemyControllers.Num(); enemyController++)
	{
		if(UKismetSystemLibrary::DoesImplementInterface(_EnemyControllers[enemyController], UResetable::StaticClass()))
		{
			IResetable::Execute_Reset(_EnemyControllers[enemyController], _EnemyStarts[enemyController]->GetActorLocation());
		}
	}
}

void AGM_FPS::ReplayGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AGM_FPS::PlayerDead()
{
	Handle_GameRuleCompleted(false);
}

void AGM_FPS::Handle_GameRulePointsScored(AController* scorer, int points)
{
	if(UKismetSystemLibrary::DoesImplementInterface(scorer, UControllable::StaticClass()))
	{
		IControllable::Execute_AddPoints(scorer, points);
	}
}

void AGM_FPS::Handle_GameRuleCompleted(bool successful)
{
	for(AController* playerController : _PlayerControllers)
	{
		if(UKismetSystemLibrary::DoesImplementInterface(playerController, UControllable::StaticClass()))
		{
			IControllable::Execute_DisablePlayerInput(playerController);

			if(successful) { _GMWidget->ShowWinScreen(); }
			else { _GMWidget->ShowLoseScreen(); }
		}
	}
}

void AGM_FPS::Handle_GameRuleSystemDepleted(ESystemType systemType)
{
	if(systemType == ESystemType::Oxygen) { Handle_GameRuleCompleted(false); }
}

