// Fill out your copyright notice in the Description page of Project Settings.


#include "InsideTargetRegion.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "EnemyAi.h"
#include "RegionArea.h"

UInsideTargetRegion::UInsideTargetRegion()
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_Point::StaticClass();
	SetWorkOnFloatValues(false);
}

void UInsideTargetRegion::RunTest(FEnvQueryInstance & QueryInstance) const
{
	AEnemyAi* Enemy = Cast<AEnemyAi>(QueryInstance.Owner.Get());

	if (!IsValid(Enemy))
		return;

	AEnemyAIController* Controller = Cast<AEnemyAIController>(Enemy->GetController());

	if (!IsValid(Controller))
		return;

	UBlackboardComponent* Board = Controller->GetBlackboardComponent();

	if (!IsValid(Board))
		return;

	static const FName TargetRegionAreaKey = TEXT("TargetRegionArea");

	ARegionArea* TargetRegion = Cast<ARegionArea>(Board->GetValueAsObject(TargetRegionAreaKey));

	if (!IsValid(TargetRegion))
		return;

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation =
			GetItemLocation(QueryInstance, It.GetIndex());

		const bool isInside = TargetRegion->IsInsideRegionArea(ItemLocation);

		It.SetScore(TestPurpose, FilterType, isInside, true);
	}
}
