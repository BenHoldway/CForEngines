#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GM_FPS.generated.h"

enum ESystemType : int;
class AAIC_FPS;
class ACustomPawnStart;
class UGameRule;

UCLASS()
class CFORENGINES_API AGM_FPS : public AGameMode
{
	GENERATED_BODY()

public:
	virtual AActor* FindPlayerStart_Implementation(AController* Character, const FString& IncomingName) override;
	
 
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
 
protected:
	int _GameRulesLeft;
UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UGameRule>> _GameRuleManagers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AActor>> _PlayerStarts;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AActor>> _EnemyStarts;

	TArray<TObjectPtr<AActor>> _UsedStarts;
	
	TArray<TObjectPtr<AController>> _PlayerControllers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AAIC_FPS> _EnemyControllerClass;

	virtual void BeginPlay() override;
	
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
 
	virtual void OnMatchStateSet() override;
 
	virtual bool ReadyToStartMatch_Implementation() override;
	virtual bool ReadyToEndMatch_Implementation() override;

private:
	UFUNCTION()
	void Handle_GameRuleCompleted(bool successful);
	UFUNCTION()
	void Handle_GameRulePointsScored(AController* scorer, int points);
	UFUNCTION()
	void Handle_GameRuleSystemDepleted(ESystemType systemType);
};
