#pragma once
 
#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_FloatLessThan.generated.h"
 
struct FBTFloatLessThanDecoratorMemory
{
	bool bLastRawResult;
};
 
UCLASS()
class CFORENGINES_API UBTDecorator_FloatLessThan : public UBTDecorator
{
	GENERATED_BODY()
	typedef FBTFloatLessThanDecoratorMemory TNodeInstanceMemory;
 
public:
	UBTDecorator_FloatLessThan(const FObjectInitializer& ObjectInitializer);
 
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override;
 
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual bool CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
 
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector Key_Value;
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector Key_Comparison;
};