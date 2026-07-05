// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ValidateCoverPointService.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UValidateCoverPointService : public UBTService
{
	GENERATED_BODY()
	
public:
	UValidateCoverPointService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& ownerCmp, uint8* nodeMemory, float deltaSecond) override;

private:
	bool IsCoveredFromTarget(FVector from, FVector to, AActor* owner);

};
