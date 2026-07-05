// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIUtilityContext.generated.h"

class AEnemyAIController;
class AFirstPersonBase;
struct FTargetData;

/**
 * 
 */
USTRUCT()
struct FAIUtilityContext
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<AEnemyAIController> Controller;

	UPROPERTY()
	TObjectPtr<AFirstPersonBase> OwnerPawn;

	const FTargetData* TargetData;

	FAIUtilityContext() = default;
	FAIUtilityContext(AEnemyAIController* controller, AFirstPersonBase* ownerPawn, const FTargetData* targetData)
		:
		Controller(controller),
		OwnerPawn(ownerPawn),
		TargetData(targetData)
	{
	}

	UPROPERTY()
	bool isInCombat = false;

};
