// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_TargetDistance.h"
#include "AIUtilityContext.h"
#include "TargetData.h"
#include "EnemyAIController.h"
#include "FirstPersonBase.h"
#include "BehaviorTree/BlackboardComponent.h"

float UAC_TargetDistance::RawScore(const FAIUtilityContext& context) const
{
	AEnemyAIController* pController = context.Controller;

	if (!IsValid(pController))
		return 0.0f;

	AFirstPersonBase* pOwnerPawn = Cast<AFirstPersonBase>( pController->GetPawn());

	if (!IsValid(pOwnerPawn))
		return 0.0f;

	UBlackboardComponent* pBoard = pController->GetBlackboardComponent();

	if (!IsValid(pBoard))
		return 0.0f;

	AFirstPersonBase* pTargetPawn = Cast<AFirstPersonBase>(pBoard->GetValueAsObject(TargetKey));

	if (!IsValid(pTargetPawn))
		return 0.0f;

	FVector ownerLocation = pOwnerPawn->GetActorLocation();
	FVector targetLocation = pTargetPawn->GetActorLocation();

	float dist = FVector::Dist(targetLocation, ownerLocation);

	return FMath::Clamp(dist / MaxDistance, 0.0f, 1.0f);
}
