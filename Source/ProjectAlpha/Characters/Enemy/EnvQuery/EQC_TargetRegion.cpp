// Fill out your copyright notice in the Description page of Project Settings.


#include "EQC_TargetRegion.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "EnemyAi.h"
#include "RegionArea.h"

UEQC_TargetRegion::UEQC_TargetRegion()
{

}

void UEQC_TargetRegion::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	AEnemyAi* enemy = Cast<AEnemyAi>(QueryInstance.Owner);

	if (!IsValid(enemy))
		return;

	AEnemyAIController* controller = Cast<AEnemyAIController>(enemy->GetController());

	if (!IsValid(controller))
		return;

	UBlackboardComponent* pBoard = controller->GetBlackboardComponent();

	if (!IsValid(pBoard))
		return;

	static const FName kTargetRegionAreaKey = TEXT("TargetRegionArea");
	ARegionArea* regionArea = Cast<ARegionArea>(pBoard->GetValueAsObject(kTargetRegionAreaKey));

	if (!IsValid(regionArea))
		return;

	FVector location = regionArea->GetActorLocation();

	UEnvQueryItemType_Point::SetContextHelper(ContextData, location);

}
