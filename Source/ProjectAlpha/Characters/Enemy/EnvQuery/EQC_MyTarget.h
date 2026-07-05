// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EQC_MyTarget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UEQC_MyTarget : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	UEQC_MyTarget();

protected:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

};
