// Fill out your copyright notice in the Description page of Project Settings.


#include "AiConsideration.h"

float UAiConsideration::Score(const FAIUtilityContext& context) const
{
    float score = RawScore(context);
    switch (EquationType)
    {
    case ScoreEquation::kNone:
        return score;
        break;
    case ScoreEquation::kLinear:
        return LinearScore(score);
        break;
    case ScoreEquation::kInvLinear:
        return 1.0f - LinearScore(score);
    case ScoreEquation::kPower:
        return PowerScore(score);
    case ScoreEquation::kInvPower:
        return 1.0f - PowerScore(score);
    case ScoreEquation::kReversePower:
        return ReversePowerScore(score);
    case ScoreEquation::kInvReversePower:
        return 1.0f - ReversePowerScore(score);
    case ScoreEquation::kRoot:
        return RootScore(score);
    case ScoreEquation::kInvRoot:
        return 1.0f - RootScore(score);
    default:
        break;
    }

    return 0.0f;
}

float UAiConsideration::LinearScore(float score) const
{
    return score;
}

float UAiConsideration::PowerScore(float score) const
{
    return FMath::Clamp(FMath::Pow(score,PowerValue),0.0f,1.0f);
}

float UAiConsideration::ReversePowerScore(float score) const
{
    float irrationalValue = FMath::Clamp(1.0f - score, 0.0f, 1.0f);
    return PowerScore(irrationalValue);
}

float UAiConsideration::RootScore(float score) const
{
    return FMath::Clamp(FMath::Pow(score, 1.0f / PowerValue), 0.0f, 1.0f);
}
