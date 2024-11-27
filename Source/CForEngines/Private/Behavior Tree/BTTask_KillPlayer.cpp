// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior Tree/BTTask_KillPlayer.h"

#include "Kismet/KismetSystemLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Components/Damageable.h"
#include "Character/Components/HealthComponent.h"


UBTTask_KillPlayer::UBTTask_KillPlayer()
{
	bNotifyTick = false;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;
 
	Key_Pawn.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_KillPlayer, Key_Pawn), APawn::StaticClass());
	Key_TargetActor.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_KillPlayer, Key_TargetActor), AActor::StaticClass());
	Key_Distance.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_KillPlayer, Key_Distance));
}

void UBTTask_KillPlayer::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
 
	UBlackboardData* behaviourTree = GetBlackboardAsset();
	if(ensure(behaviourTree))
	{
		Key_Pawn.ResolveSelectedKey(*behaviourTree);
		Key_TargetActor.ResolveSelectedKey(*behaviourTree);
		Key_Distance.ResolveSelectedKey(*behaviourTree);
	}
}

EBTNodeResult::Type UBTTask_KillPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* behaviourTree = OwnerComp.GetBlackboardComponent();
	if(behaviourTree->GetValueAsFloat(Key_Distance.SelectedKeyName) <= 1.0f) { return EBTNodeResult::Failed; }
	
	AActor* thisActor = Cast<AActor>(behaviourTree->GetValueAsObject(Key_Pawn.SelectedKeyName));
	AActor* targetActor = Cast<AActor>(behaviourTree->GetValueAsObject(Key_TargetActor.SelectedKeyName));

	UHealthComponent* targetActorHealth = targetActor->GetComponentByClass<UHealthComponent>();
	if(UKismetSystemLibrary::DoesImplementInterface(targetActorHealth, UDamageable::StaticClass()))
	{
		float dis = behaviourTree->GetValueAsFloat("Distance");
		UE_LOG(LogTemp, Display, TEXT("Distance: %f"), dis);
		IDamageable::Execute_Damage(targetActorHealth, thisActor->GetInstigatorController());
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}
