// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_LocationDistance.h"
#include "AIUtilityContext.h"
#include "TargetData.h"
#include "EnemyAIController.h"
#include "FirstPersonBase.h"
#include "BehaviorTree/BlackboardComponent.h"

float UAC_LocationDistance::RawScore(const FAIUtilityContext& context) const
{
	AEnemyAIController* pController = context.Controller;

	if (!IsValid(pController))
		return 0.0f;

	AFirstPersonBase* pOwnerPawn = Cast<AFirstPersonBase>(pController->GetPawn());

	if (!IsValid(pOwnerPawn))
		return 0.0f;

	UBlackboardComponent* pBoard = pController->GetBlackboardComponent();

	if (!IsValid(pBoard))
		return 0.0f;

	FVector location = pBoard->GetValueAsVector(TargetKey);


	FVector ownerLocation = pOwnerPawn->GetActorLocation();

	float dist = FVector::Dist(location, ownerLocation);

	return FMath::Clamp(dist / MaxDistance, 0.0f, 1.0f);
}
