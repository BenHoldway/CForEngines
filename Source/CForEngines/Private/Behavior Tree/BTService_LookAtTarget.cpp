// Fill out your copyright notice in the Description page of Project Settings.


#include "Behavior Tree/BTService_LookAtTarget.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTService_LookAtTarget::UBTService_LookAtTarget()
{
	bNotifyTick = true;
	bNotifyBecomeRelevant = false;
	bNotifyCeaseRelevant = false;
 
	Key_Pawn.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_LookAtTarget, Key_Pawn), APawn::StaticClass());
	Key_TargetActor.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_LookAtTarget, Key_TargetActor), AActor::StaticClass());
	Key_WanderPos.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_LookAtTarget, Key_WanderPos));
	Key_RotateTime.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_LookAtTarget, Key_RotateTime));
}

void UBTService_LookAtTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* behaviourTree = GetBlackboardAsset();
	if(ensure(behaviourTree))
	{
		Key_Pawn.ResolveSelectedKey(*behaviourTree);
		Key_TargetActor.ResolveSelectedKey(*behaviourTree);
		Key_WanderPos.ResolveSelectedKey(*behaviourTree);
		Key_RotateTime.ResolveSelectedKey(*behaviourTree);
	}
}

void UBTService_LookAtTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* behaviourTree = OwnerComp.GetBlackboardComponent();
	AActor* thisActor = Cast<AActor>(behaviourTree->GetValueAsObject(Key_Pawn.SelectedKeyName));
	AActor* targetActor = Cast<AActor>(behaviourTree->GetValueAsObject(Key_TargetActor.SelectedKeyName));

	if(behaviourTree == nullptr || thisActor == nullptr) { return; }

	FVector dir;
	if(targetActor == nullptr) { dir = behaviourTree->GetValueAsVector(Key_WanderPos.SelectedKeyName) - thisActor->GetActorLocation(); }
	else { dir = targetActor->GetActorLocation() - thisActor->GetActorLocation(); }

	FRotator newRotator = FRotator(0.0f, dir.Rotation().Yaw, 0.0f);
	//thisActor->SetActorRotation(FRotator(FQuat::Slerp(thisActor->GetActorQuat(), FQuat::MakeFromRotator(newRotator), behaviourTree->GetValueAsFloat(Key_RotateTime.SelectedKeyName) * DeltaSeconds)));
	thisActor->SetActorRotation(FRotator(0.0f, dir.Rotation().Yaw, 0.0f));
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
