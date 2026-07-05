// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "UpdateTargetService.generated.h"

class AEnemyAIController;
class UBlackboardComponent;

/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UUpdateTargetService : public UBTService
{
	GENERATED_BODY()
public:
	UUpdateTargetService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& ownerCmp, uint8* nodeMemory, float deltaSecond) override;

private:
	void UpdateTarget(AEnemyAIController* pController, UBlackboardComponent* pBoard);
};
