// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "InsideTargetRegion.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UInsideTargetRegion : public UEnvQueryTest
{
	GENERATED_BODY()
public:
	UInsideTargetRegion();

protected:
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
