// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "UtilityAI.generated.h"

class AEnemyAIController;
class UAiDecision;

/**
 * 
 */

struct FScoredDecision
{
	TObjectPtr<UAiDecision> Decision = nullptr;
	float Score = 0.0f;

	FScoredDecision() = default;

	FScoredDecision(UAiDecision* decision, float score)
		: Decision(decision)
		, Score(score)
	{}
};


UCLASS()
class PROJECTALPHA_API UUtilityAI : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	static constexpr float kRandomCutoff = 0.5f;
	static constexpr float kSlicingTime = 0.2f;
public:
	void AddToDecisionQueue(AEnemyAIController* controller);

protected:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;

	void RunUtility(TWeakObjectPtr<AEnemyAIController> controllerPtr);

private:
	TQueue<TWeakObjectPtr<AEnemyAIController>> m_aiDecisionQueue;

};
