// Fill out your copyright notice in the Description page of Project Settings.


#include "FindSearchLootAreaTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "EnemyAi.h"
#include "RegionArea.h"

EBTNodeResult::Type UFindSearchLootAreaTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AEnemyAIController* pController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

    if (!IsValid(pController))
        return EBTNodeResult::Failed;

    AEnemyAi* pPawn = Cast<AEnemyAi>(pController->GetPawn());

    if (!IsValid(pPawn))
        return EBTNodeResult::Failed;

    ARegionArea* pLocatedRegion = pPawn->GetLocatedRegion();

    if(!IsValid(pLocatedRegion))
        return EBTNodeResult::Failed;

    const TArray<TObjectPtr<ARegionArea>>& connectedAreas = pLocatedRegion->GetConnectedAreas();

    if(connectedAreas.IsEmpty())
        return EBTNodeResult::Failed;

    ARegionArea* bestArea = pController->GetBestSearchRegion(connectedAreas);

    UBlackboardComponent* pBoard = OwnerComp.GetBlackboardComponent();

    if(!IsValid(pBoard))
        return EBTNodeResult::Failed;

    pBoard->SetValueAsObject(BlackboardKey.SelectedKeyName, bestArea);
    
    return EBTNodeResult::Succeeded;


}
