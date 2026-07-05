// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EQC_VisibleTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UEQC_VisibleTarget : public UEnvQueryContext
{
	GENERATED_BODY()


protected:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
