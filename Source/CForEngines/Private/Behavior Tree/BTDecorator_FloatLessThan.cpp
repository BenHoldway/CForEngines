#include "Behavior Tree/BTDecorator_FloatLessThan.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_FloatLessThan::UBTDecorator_FloatLessThan(const FObjectInitializer& ObjectInitializer)
{
    Key_Value.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_FloatLessThan, Key_Value));
    Key_Comparison.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_FloatLessThan, Key_Comparison));
 
    INIT_DECORATOR_NODE_NOTIFY_FLAGS();
}
 
void UBTDecorator_FloatLessThan::InitializeFromAsset(UBehaviorTree& Asset)
{
    Super::InitializeFromAsset(Asset);
    
    UBlackboardData* BBAsset = GetBlackboardAsset();
    if(ensure(BBAsset))
    {
       Key_Value.ResolveSelectedKey(*BBAsset);
       Key_Comparison.ResolveSelectedKey(*BBAsset);
    }
}
 
uint16 UBTDecorator_FloatLessThan::GetInstanceMemorySize() const
{
    return sizeof(FBTFloatLessThanDecoratorMemory);
}
 
void UBTDecorator_FloatLessThan::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    TNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<TNodeInstanceMemory>(NodeMemory);
 
    const bool bResult = CalcConditionImpl(OwnerComp, NodeMemory);
    if(bResult != DecoratorMemory->bLastRawResult)
    {
       DecoratorMemory->bLastRawResult = bResult;
       OwnerComp.RequestExecution(this);
    }
}
 
bool UBTDecorator_FloatLessThan::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    return CalcConditionImpl(OwnerComp, NodeMemory);
}
 
bool UBTDecorator_FloatLessThan::CalcConditionImpl(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    return BBComp->GetValueAsFloat(Key_Value.SelectedKeyName) < BBComp->GetValueAsFloat(Key_Comparison.SelectedKeyName);
}
 
void UBTDecorator_FloatLessThan::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    TNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<TNodeInstanceMemory>(NodeMemory);
    DecoratorMemory->bLastRawResult = CalcConditionImpl(OwnerComp, NodeMemory);
}