// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AiConsideration.h"
#include "AC_InventoryCondition.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UAC_InventoryCondition : public UAiConsideration
{
	GENERATED_BODY()
public:
	virtual float RawScore(const FAIUtilityContext& context) const override;
};
