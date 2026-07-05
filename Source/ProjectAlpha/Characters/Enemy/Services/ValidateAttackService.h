// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ValidateAttackService.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UValidateAttackService : public UBTService
{
	GENERATED_BODY()
public:
	UValidateAttackService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& ownerCmp, uint8* nodeMemory, float deltaSecond) override;

private:
	bool IsTargetVisible(FVector from, FVector to, AActor* owner);

};
