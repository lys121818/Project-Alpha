// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ChooseBestLootTask.generated.h"

class UItemData;
class UWeaponItem;
class AFirstPersonBase;

/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UChooseBestLootTask : public UBTTaskNode
{
	GENERATED_BODY()

	static constexpr int kMaxHealkitCount = 5;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Weight")
	float NeedWeight = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Weight")
	float ValueWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Weight")
	float DistanceWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance Score")
	float MaxDistanceValue = 1000.0f;
private:
	float NeedScore(const UItemData* item, const AFirstPersonBase* ownerPawn);
	float ValueScore(const UItemData* item);
	float DistanceScore(const AActor* item, const AFirstPersonBase* ownerPawn);

	float WeaponNeedScore(const UWeaponItem* pOwnedWeapon, const UWeaponItem* pNewWeapon);
	float AmmoNeedScore(const AFirstPersonBase* ownerPawn, const UItemData* item) const;
	float HealthKitNeedScore(const AFirstPersonBase* ownerPawn, const UItemData* item) const;
	

};
