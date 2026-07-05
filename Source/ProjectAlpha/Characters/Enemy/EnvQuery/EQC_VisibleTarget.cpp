// Fill out your copyright notice in the Description page of Project Settings.


#include "EQC_VisibleTarget.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"

void UEQC_VisibleTarget::ProvideContext(FEnvQueryInstance & QueryInstance, FEnvQueryContextData & ContextData) const
{
	// set visible target as context

	AEnemyAIController* controller = Cast<AEnemyAIController>(QueryInstance.Owner);

	if (!controller)
		return;

	UBlackboardComponent* pBoard = controller->GetBlackboardComponent();

	if (!pBoard)
		return;

	static const FName kVisibleTargetKey = TEXT("VisibleTarget");
	const AActor* target = Cast<AActor>(pBoard->GetValueAsObject(kVisibleTargetKey));

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, target);
}
