// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdateTargetService.h"

#include "EnemyAIController.h"
#include "FirstPersonBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UUpdateTargetService::UUpdateTargetService()
{
	NodeName = TEXT("Update Targets");

	bNotifyTick = true;
}

void UUpdateTargetService::TickNode(UBehaviorTreeComponent& ownerCmp, uint8* nodeMemory, float deltaSecond)
{
	AEnemyAIController* controller = Cast<AEnemyAIController>(ownerCmp.GetAIOwner());

	if (!IsValid(controller))
		return;

	UBlackboardComponent* pBoard = controller->GetBlackboardComponent();

	if (!IsValid(pBoard))
		return;

	UpdateTarget(controller, pBoard);


}

void UUpdateTargetService::UpdateTarget(AEnemyAIController* pController, UBlackboardComponent* pBoard)
{
	const TArray<FDamagedActor>& causedTargets = pController->GetCausedTargets();
	const TArray<TWeakObjectPtr<AActor>>& visibleTargets = pController->GetVisibleTargets();
	AFirstPersonBase* pPawn = Cast<AFirstPersonBase>(pController->GetPawn());

	if (!IsValid(pPawn))
		return;

	const FVector ownerLocation = pPawn->GetActorLocation();

	const FName kCausedTargetKey = TEXT("CausedTarget");
	const FName kVisibleTargetKey = TEXT("VisibleTarget");
	const FName kTargetCharacterKey = TEXT("TargetCharacter");
	const FName kLastKnownLocationKey = TEXT("LastKnownTargetLocation");
	const FName kTargetItemKey = TEXT("TargetItem");
	const FName kIsInCombatKey = TEXT("IsInCombat");

	// clear invalid item
	UObject* pTargetItem = pBoard->GetValueAsObject(kTargetItemKey);

	pBoard->SetValueAsBool(kIsInCombatKey, pPawn->IsInCombat());

	if (!IsValid(pTargetItem))
		pBoard->ClearValue(kTargetItemKey);

	AActor* bestCausedTarget = nullptr;
	float bestCausedDist = FLT_MAX;

	// caused target
	for (const auto& target : causedTargets)
	{
		AActor* targetPawn = target.Actor.Get();
		if (!IsValid(targetPawn))
			continue;

		float dist = FVector::Dist(targetPawn->GetActorLocation(), ownerLocation);
		if (dist < bestCausedDist)
		{
			bestCausedDist = dist;
			bestCausedTarget = targetPawn;
		}
	}

	AActor* bestVisibleTarget = nullptr;
	float bestVisibleDist = FLT_MAX;

	// visible target
	for (const TWeakObjectPtr<AActor>& target : visibleTargets)
	{
		AActor* actor = target.Get();
		if (!IsValid(actor))
			continue;

		float dist = FVector::Dist(actor->GetActorLocation(), ownerLocation);
		if (dist < bestVisibleDist)
		{
			bestVisibleDist = dist;
			bestVisibleTarget = actor;
		}
	}

	if (IsValid(bestCausedTarget))
	{
		pBoard->SetValueAsObject(kCausedTargetKey, bestCausedTarget);
		pBoard->SetValueAsObject(kTargetCharacterKey, bestCausedTarget);
		pBoard->SetValueAsVector(kLastKnownLocationKey, bestCausedTarget->GetActorLocation());
		pPawn->SetCombat(true);
	}
	else
	{
		pBoard->ClearValue(kCausedTargetKey);

		if (IsValid(bestVisibleTarget))
		{
			pBoard->SetValueAsObject(kTargetCharacterKey, bestVisibleTarget);
			pBoard->SetValueAsVector(kLastKnownLocationKey, bestVisibleTarget->GetActorLocation());
			pPawn->SetCombat(true);
		}
		else
		{
			pBoard->ClearValue(kTargetCharacterKey);
			pPawn->SetCombat(false);
		}
	}

	if (IsValid(bestVisibleTarget))
		pBoard->SetValueAsObject(kVisibleTargetKey, bestVisibleTarget);
	else
		pBoard->ClearValue(kVisibleTargetKey);


}
