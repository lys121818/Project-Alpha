// Fill out your copyright notice in the Description page of Project Settings.


#include "TryPickupItemTask.h"
#include "EnemyAi.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PickupItem.h"

EBTNodeResult::Type UTryPickupItemTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get controller of the ai actor object
	AAIController* aiController = OwnerComp.GetAIOwner();

	if (!aiController)
		return EBTNodeResult::Failed;

	// get target pawn
	AEnemyAi* enemyAi = Cast<AEnemyAi>(aiController->GetPawn());
	if (!enemyAi)
		return EBTNodeResult::Failed;

	UBlackboardComponent* pBoard = OwnerComp.GetBlackboardComponent();

	if (!pBoard)
		return EBTNodeResult::Failed;

	static const FName kTargetItemKey = TEXT("TargetItem");

	APickupItem* item = Cast<APickupItem>(pBoard->GetValueAsObject(kTargetItemKey));

	// pickup item
	if(enemyAi->TryPickupItem(item))
		return EBTNodeResult::Succeeded;

	return EBTNodeResult::Failed;
}
