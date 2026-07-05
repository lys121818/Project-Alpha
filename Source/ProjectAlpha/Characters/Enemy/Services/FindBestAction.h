// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "FindBestAction.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UFindBestAction : public UBTService
{
	GENERATED_BODY()
public:
	UFindBestAction();

protected:
	virtual void TickNode(UBehaviorTreeComponent& ownerCmp, uint8* nodeMemory, float deltaSecond) override;

};
