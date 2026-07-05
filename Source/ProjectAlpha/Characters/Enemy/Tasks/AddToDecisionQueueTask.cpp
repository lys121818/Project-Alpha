// Fill out your copyright notice in the Description page of Project Settings.


#include "AddToDecisionQueueTask.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "EnemyAIController.h"

EBTNodeResult::Type UAddToDecisionQueueTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AEnemyAIController* pController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

    if (!IsValid(pController))
        return EBTNodeResult::Failed;

    pController->AddToDecisionQueue();

    return EBTNodeResult::Succeeded;
}
