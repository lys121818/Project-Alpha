// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AiConsideration.h"
#include "AC_LoadedAmmoCondition.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UAC_LoadedAmmoCondition : public UAiConsideration
{
	GENERATED_BODY()
	
	virtual float RawScore(const FAIUtilityContext& context) const override;

};
