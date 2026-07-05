// Fill out your copyright notice in the Description page of Project Settings.


#include "UseHealKitTask.h"
#include "FirstPersonBase.h"
#include "EnemyAIController.h"

EBTNodeResult::Type UUseHealKitTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* pController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(pController))
		return EBTNodeResult::Failed;

	AFirstPersonBase* pOwnerPawn = Cast<AFirstPersonBase>(pController->GetPawn());

	if (!IsValid(pOwnerPawn))
		return EBTNodeResult::Failed;

	if(pOwnerPawn->TryUseHealKitItem())
		return EBTNodeResult::Succeeded;

	return EBTNodeResult::InProgress;

}
