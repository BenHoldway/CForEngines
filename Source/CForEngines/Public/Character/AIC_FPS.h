#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIC_FPS.generated.h"

class UAISense_Sight;
class UAIPerceptionComponent;
class APawn;

UCLASS(Abstract)
class CFORENGINES_API AAIC_FPS : public AAIController
{
	GENERATED_BODY()

public:
	AAIC_FPS();

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAIPerceptionComponent> _AIPerception;
	TObjectPtr<UAISense_Sight> _AISense_Sight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APawn> _EnemyPawn;

private:
	UFUNCTION()
	void Handle_PerceptionUpdated();
};
