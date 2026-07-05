// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ChooseLootItemService.generated.h"

class UItemData;
class UWeaponItem;
class AFirstPersonBase;
class AEnemyAIController;

/**
 *	This class tick function can be heavy, recomend to run with significant amount of interval as it needs it
 */
UCLASS()
class PROJECTALPHA_API UChooseLootItemService : public UBTService
{
	GENERATED_BODY()

	static constexpr int kMaxHealkitCount = 5;
public:
	UChooseLootItemService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& ownerCmp, uint8* nodeMemory, float deltaSecond) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Weight")
	float ValueWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Weight")
	float DistanceWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score Weight")
	float DangerWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance Score")
	float MaxDistanceValue = 1000.0f;

private:
	float NeedScore(const UItemData* item, const AFirstPersonBase* ownerPawn);
	float ValueScore(const UItemData* item);
	float DistanceScore(const AActor* item, const AFirstPersonBase* ownerPawn);
	float DangerScore(const AActor* item, const AEnemyAIController* controller);

	float WeaponNeedScore(const UWeaponItem* pOwnedWeapon, const UWeaponItem* pNewWeapon);
	float AmmoNeedScore(const AFirstPersonBase* ownerPawn, const UItemData* item) const;
	float HealthKitNeedScore(const AFirstPersonBase* ownerPawn, const UItemData* item) const;

};
