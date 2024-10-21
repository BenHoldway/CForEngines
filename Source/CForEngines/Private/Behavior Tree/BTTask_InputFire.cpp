#include "Behavior Tree/BTTask_InputFire.h"

#include "Character/Components/Inputable.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
 
UBTTask_InputFire::UBTTask_InputFire()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;
 
	Key_Pawn.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_InputFire, Key_Pawn), APawn::StaticClass());
}
 
void UBTTask_InputFire::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
 
	UBlackboardData* BBAsset = GetBlackboardAsset();
	if(ensure(BBAsset))
	{
		Key_Pawn.ResolveSelectedKey(*BBAsset);
	}
}
 
EBTNodeResult::Type UBTTask_InputFire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	UObject* pawn = BBComp->GetValueAsObject(Key_Pawn.SelectedKeyName);
	if(UKismetSystemLibrary::DoesImplementInterface(pawn, UInputable::StaticClass()))
	{
		IInputable::Execute_Input_FirePressed(pawn);
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}
 
EBTNodeResult::Type UBTTask_InputFire::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	UObject* pawn = BBComp->GetValueAsObject(Key_Pawn.SelectedKeyName);
	if(UKismetSystemLibrary::DoesImplementInterface(pawn, UInputable::StaticClass()))
	{
		IInputable::Execute_Input_FireReleased(pawn);
	}
	return Super::AbortTask(OwnerComp, NodeMemory);
}