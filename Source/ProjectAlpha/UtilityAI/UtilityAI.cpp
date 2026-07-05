// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAI.h"
#include "EnemyAiController.h"
#include "Containers/Queue.h"
#include "AIUtilityContext.h"
#include "EnemyAi.h"
#include "AiDecision.h"
#include "UtilityDebugScore.h"

void UUtilityAI::AddToDecisionQueue(AEnemyAIController* controller)
{
	if (!IsValid(controller))
		return;

	m_aiDecisionQueue.Enqueue(controller);
}

void UUtilityAI::Tick(float DeltaTime)
{
	float startTime = FPlatformTime::Seconds();

	while (!m_aiDecisionQueue.IsEmpty())
	{
		float timeSpent = FPlatformTime::Seconds() - startTime;

		// time slice break down
		if (timeSpent > kSlicingTime)
			break;

		TWeakObjectPtr<AEnemyAIController> controller;
		if (m_aiDecisionQueue.Dequeue(controller))
		{
			if (!IsValid(controller.Get()))
				continue;

			RunUtility(controller);

		}
	}
}

TStatId UUtilityAI::GetStatId() const
{

	return Super::GetStatID();
}

bool UUtilityAI::IsTickable() const
{
	return (GetWorld() != nullptr);
}

void UUtilityAI::RunUtility(TWeakObjectPtr<AEnemyAIController> controllerPtr)
{
	AEnemyAIController* controller = controllerPtr.Get();

	if (!IsValid(controller))
		return;

	AEnemyAi* ownerPawn = Cast<AEnemyAi>(controller->GetPawn());

	if (!IsValid(ownerPawn))
		return;

	const FTargetData* data = controller->GetTargetData();

	if (data == nullptr)
		return;

	const TArray<TObjectPtr<UAiDecision>>& decisions = ownerPawn->GetDecisions();

	if (decisions.IsEmpty())
		return;

	// Run Decisions in controller
	FAIUtilityContext context = FAIUtilityContext(controller, ownerPawn, data);
	TArray<FScoredDecision> scoredDecisions;

	float total = 0.0f;
	float bestScore = FLT_MIN;

	//Debug
	controller->ClearLastDebugScores();


	// calculate decision score
	for (const auto& decision : decisions)
	{
		FUtilityDebugScore utiltityScore;

		float score = decision->Score(context, &utiltityScore);

		if (ownerPawn->IsInCombat())
			score *= decision->GetCombatMultiplier();

		//Debug
		utiltityScore.SetDecisionName(decision->GetDecisionName());
		utiltityScore.SetScore(score);
		controller->AddLastDebugScores(utiltityScore);

		if (score <= 0.0f)
			continue;


		scoredDecisions.Emplace(decision.Get(), score);
		total += score;

		if (score > bestScore)
		{
			bestScore = score;
		}

	}

	if (scoredDecisions.IsEmpty())
		return;

	float scorePosition = 0.0f;
	float cutoffScore = kRandomCutoff * bestScore;

	// remove 
	for (int i = 0; i < scoredDecisions.Num();)
	{
		if (scoredDecisions[i].Score < cutoffScore)
		{
			total -= scoredDecisions[i].Score;

			scoredDecisions.RemoveAt(i);
			continue;
		}

		i++;
	}

	if (scoredDecisions.IsEmpty() || total <= 0.0f)
		return;

	const float randomValue = FMath::FRandRange(0.0f, total);


	// Weighted Random decision select
	for (const auto& scoredDecision : scoredDecisions)
	{
		scorePosition += scoredDecision.Score;

		if (scorePosition >= randomValue)
		{
			UAiDecision* selectedDecision = scoredDecision.Decision.Get();
			if (IsValid(selectedDecision))
			{
				selectedDecision->Execute(context);

				//Debug
				controller->SetLastSelectedAction(selectedDecision->GetDecisionName());
			}

			break;
		}
	}

}
