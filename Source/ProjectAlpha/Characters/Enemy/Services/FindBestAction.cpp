// Fill out your copyright notice in the Description page of Project Settings.


#include "FindBestAction.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ActionType.h"

#include "EnemyAi.h"
#include "EnemyAIController.h"
#include "UtilityAI.h"

UFindBestAction::UFindBestAction()
{

}

void UFindBestAction::TickNode(UBehaviorTreeComponent & ownerCmp, uint8 * nodeMemory, float deltaSecond)
{
    Super::TickNode(ownerCmp, nodeMemory, deltaSecond);

    AEnemyAIController* pController = Cast<AEnemyAIController>(ownerCmp.GetAIOwner());

    if (!IsValid(pController))
        return;

    if (pController->IsReadyForDecisionQueue())
    {
        pController->AddToDecisionQueue();
    }

}
