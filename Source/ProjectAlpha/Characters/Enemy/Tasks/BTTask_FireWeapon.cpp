// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FireWeapon.h"
#include "EnemyAi.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"


EBTNodeResult::Type UBTTask_FireWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get controller of the ai actor object
	AEnemyAIController* aiController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (!aiController)
		return EBTNodeResult::Failed;

	// get target pawn
	AEnemyAi* enemyAi = Cast<AEnemyAi>(aiController->GetPawn());
	if(!enemyAi)
		return EBTNodeResult::Failed;

	UBlackboardComponent* BBoard = OwnerComp.GetBlackboardComponent();

	// get target information from black board
	AActor* target = Cast<AActor>(BBoard->GetValueAsObject(TEXT("TargetCharacter")));

	aiController->AddCausedTarget(target);

	if (enemyAi->TryFireAt(target))
		return EBTNodeResult::Succeeded;

	return EBTNodeResult::Failed;
}
