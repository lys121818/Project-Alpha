// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../ActionType.h"
#include "AiDecision.generated.h"

class UAiConsideration;
struct FAIUtilityContext;
struct FUtilityDebugScore;
/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTALPHA_API UAiDecision : public UDataAsset
{
	GENERATED_BODY()

public:
	float Score(const FAIUtilityContext& context, FUtilityDebugScore* debugScore = nullptr) const;
	void Execute(const FAIUtilityContext& context) const;
	FName GetDecisionName() const { return ActionName; }
	float GetCombatMultiplier() const { return CombatMultiplier; }

protected:

	UPROPERTY(EditDefaultsOnly)
	FName ActionName;

	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UAiConsideration>> RequiredConsiderations;

	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UAiConsideration>> Considerations;

	UPROPERTY(EditDefaultsOnly)
	EActionType ActionType;

	UPROPERTY(EditDefaultsOnly)
	float CombatMultiplier;
};
