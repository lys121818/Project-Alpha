// Fill out your copyright notice in the Description page of Project Settings.


#include "AiDecision.h"
#include "AiConsideration.h"
#include "AIUtilityContext.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UtilityDebugScore.h"

float UAiDecision::Score(const FAIUtilityContext& context, FUtilityDebugScore* debugScore) const
{
    // Required Check
    for (const UAiConsideration* consideration : RequiredConsiderations)
    {
        if (!IsValid(consideration))
            continue;

        const float score = consideration->Score(context);

        if (score <= 0.0f)
        {
            // Debug
            if (debugScore != nullptr)
            {
                debugScore->SetIsBlockedByRequired(true);
                debugScore->SetBlockedRequiredName(consideration->GetName());
            }

            return 0.0f;
        }
    }


    int considerationCount = 0;
    float totalScore = 1.0f;
    float totalWeight = 0.0f;

    // Consideration Socres
    for (const UAiConsideration* consideration : Considerations)
    {
        if (!IsValid(consideration))
            continue;

        const float rawScore = consideration->Score(context);
        const float weight = consideration->GetWeight();
        const float weightedScore = FMath::Pow(rawScore, weight);

        totalScore *= weightedScore;
        totalWeight += weight;
        considerationCount++;

        // Debug
        if (debugScore != nullptr)
        {
            FConsiderationDebugScore considerationScore;
            considerationScore.ConsiderationName = consideration->GetName();
            considerationScore.RawScore = rawScore;
            considerationScore.WeightedScore = weightedScore;

            debugScore->AddConsiderationScore(considerationScore);
        }
    }

    if (considerationCount == 0 || totalWeight <= 0.0f || totalScore <= 0.0f)
        return 0.0f;

    return FMath::Pow(totalScore, 1.0f / totalWeight);
}

void UAiDecision::Execute(const FAIUtilityContext& context) const
{
    AEnemyAIController* controller = context.Controller;
    if (!IsValid(controller))
        return;

    UBlackboardComponent* pBoard = controller->GetBlackboardComponent();

     if(!IsValid(pBoard))
        return;

    static const FName ActionTypeKey = TEXT("ActionType");

    pBoard->SetValueAsEnum(ActionTypeKey, static_cast<uint8>(ActionType));
}
