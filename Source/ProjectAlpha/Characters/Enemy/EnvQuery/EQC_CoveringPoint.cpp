// Fill out your copyright notice in the Description page of Project Settings.


#include "EQC_CoveringPoint.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "EnemyAi.h"

void UEQC_CoveringPoint::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	// set cuased target as context
	AEnemyAi* enemy = Cast<AEnemyAi>(QueryInstance.Owner);

	if (!enemy)
		return;

	AEnemyAIController* controller = Cast<AEnemyAIController>(enemy->GetController());

	if (!controller)
		return;

	UBlackboardComponent* pBoard = controller->GetBlackboardComponent();

	if (!pBoard)
		return;

	static const FName kCoveringLocationKey = TEXT("CoveringLocation");
	const FVector location = pBoard->GetValueAsVector(kCoveringLocationKey);

	UEnvQueryItemType_Point::SetContextHelper(ContextData, location);
	
}
