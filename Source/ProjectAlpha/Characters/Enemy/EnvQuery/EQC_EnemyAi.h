// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EQC_EnemyAi.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UEQC_EnemyAi : public UEnvQueryContext
{
	GENERATED_BODY()
	
public:
	UEQC_EnemyAi();

protected:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

};
