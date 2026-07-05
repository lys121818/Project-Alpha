// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UtilityDebugScore.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FConsiderationDebugScore
{
    GENERATED_BODY()

    UPROPERTY()
    FName ConsiderationName;

    UPROPERTY()
    float RawScore = 0.0f;

    UPROPERTY()
    float WeightedScore = 0.0f;

};

USTRUCT()
struct FUtilityDebugScore
{
    GENERATED_BODY()
private:
    UPROPERTY()
    FName DecisionName;

    UPROPERTY()
    float Score = 0.0f;

    UPROPERTY()
    TArray<FConsiderationDebugScore> m_considerationScores;

    UPROPERTY()
    bool IsBlockedByRequired = false;

    UPROPERTY()
    FName BlockedRequiredName;

public:
    void SetDecisionName(FName name) { DecisionName = name; }
    void SetScore(float score) { Score = score; }
    void AddConsiderationScore(FConsiderationDebugScore considerationScore) { m_considerationScores.Add(considerationScore); }
    void SetIsBlockedByRequired(bool isBlocked) { IsBlockedByRequired = isBlocked; }
    void SetBlockedRequiredName(FName name) { BlockedRequiredName = name; }

    bool GetIsBlockedByRequired() const { return IsBlockedByRequired; }
    const FName& GetBlockedRequiredName() const { return BlockedRequiredName; }
    const TArray<FConsiderationDebugScore>& GetConsiderationScores() const { return m_considerationScores; }
    FName GetDecisionName() const { return DecisionName; }
    float GetScore() const { return Score; }

};
