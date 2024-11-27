// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_KillPlayer.generated.h"

/**
 * 
 */
UCLASS()
class CFORENGINES_API UBTTask_KillPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_KillPlayer();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
 
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
 
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector Key_Pawn;
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector Key_TargetActor;
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector Key_Distance;
};
