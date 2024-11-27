#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_LookAtTarget.generated.h"

UCLASS()
class CFORENGINES_API UBTService_LookAtTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_LookAtTarget();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector Key_Pawn;
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector Key_TargetActor;
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector Key_WanderPos;
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector Key_RotateTime;
};
