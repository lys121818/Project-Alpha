// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AiConsideration.generated.h"

struct FAIUtilityContext;

UENUM()
enum class ScoreEquation
{
	kNone,
	kLinear,
	kInvLinear,
	kPower,
	kInvPower,
	kReversePower,
	kInvReversePower,
	kRoot,
	kInvRoot
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class PROJECTALPHA_API UAiConsideration : public UDataAsset
{
	GENERATED_BODY()
	
public:
	virtual float Score(const FAIUtilityContext& context) const;
	
	virtual float GetWeight() const { return Weight; }
	virtual FName GetName() const { return Name; }
protected:
	virtual float RawScore(const FAIUtilityContext& context) const PURE_VIRTUAL(UAiConsideration::RawScore, return 0.0f;);


	virtual float LinearScore(float score) const;
	virtual float PowerScore(float score) const;
	virtual float ReversePowerScore(float score) const;
	virtual float RootScore(float score) const;

protected:
	UPROPERTY(EditAnywhere)
	FName Name;

	UPROPERTY(EditAnywhere)
	float Weight = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	ScoreEquation EquationType = ScoreEquation::kNone;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "EquationType == ScoreEquation::kPower || EquationType == ScoreEquation::kInvPower || EquationType == ScoreEquation::kReversePower || EquationType == ScoreEquation::kInvReversePower || EquationType == ScoreEquation::kRoot || EquationType == ScoreEquation::kInvRoot", EditConditionHides))
	float PowerValue = 2.0f;

};
