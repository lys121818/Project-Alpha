// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EQC_TargetRegion.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UEQC_TargetRegion : public UEnvQueryContext
{
	GENERATED_BODY()
public:
	UEQC_TargetRegion();

protected:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
