// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseBestLootTask.h"
#include "FirstPersonBase.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TargetData.h"
#include "PickupItem.h"
#include "ItemData.h"
#include "WeaponItem.h"

EBTNodeResult::Type UChooseBestLootTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* pController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (!IsValid(pController))
		return EBTNodeResult::Failed;

	AFirstPersonBase* pOwnerPawn = Cast<AFirstPersonBase>(pController->GetPawn());

	if (!IsValid(pOwnerPawn))
		return EBTNodeResult::Failed;

	UBlackboardComponent* pBoard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(pBoard))
		return EBTNodeResult::Failed;

	static const FName kTargetItemKey = TEXT("TargetItem");

	const FTargetData* targetData = pController->GetTargetData();

	if(targetData == nullptr)
		return EBTNodeResult::Failed;

	const TArray<FFoundItem>& pickupItems = targetData->GetFoundItems();

	if(pickupItems.IsEmpty())
		return EBTNodeResult::Failed;

	float totalScore = 0.0f;

	float bestNeedScore = FLT_MIN;//std::numeric_limits<float>::min();
	TObjectPtr<UObject> bestItem = nullptr;

	// Find best scored item
	for (const auto& pickup : pickupItems)
	{
		APickupItem* pPickup = Cast<APickupItem>(pickup.Item);

		if (!IsValid(pPickup))
			continue;

		const UItemData* item = pPickup->GetItemData();

		if (!IsValid(item))
			continue;

		float needScore = NeedScore(item, pOwnerPawn);
		float valueScore = ValueScore(item);
		float distScore = DistanceScore(pPickup, pOwnerPawn);
		float totalWeight = NeedWeight + ValueWeight + DistanceWeight;

		if (totalWeight <= 0.0f)
			continue;

		float finalScore = ((needScore * NeedWeight) + (valueScore * ValueWeight) + (distScore * DistanceWeight)) / totalWeight;

		if (finalScore > bestNeedScore)
		{
			bestNeedScore = finalScore;
			bestItem = pPickup;
		}
	}

	if(!IsValid(bestItem))
		return EBTNodeResult::Failed;

	pBoard->SetValueAsObject(kTargetItemKey, bestItem);

	return EBTNodeResult::Succeeded;
}

// Compair with owning best item from inventory
float UChooseBestLootTask::NeedScore(const UItemData* item, const AFirstPersonBase* ownerPawn)
{


	switch (item->m_itemType)
	{
		case EItemType::kAmmo:
		{
			return AmmoNeedScore(ownerPawn, item);
			break;
		}
		case EItemType::kEquippable:
		{
			const FInventorySlot& weaponSlot = ownerPawn->GetEquipSlot(EEquipType::kWeapon);
			const UWeaponItem* ownWeapon = Cast<UWeaponItem>(weaponSlot.GetItem());
			const UWeaponItem* newWeapon = Cast<UWeaponItem>(item);
			return WeaponNeedScore(ownWeapon, newWeapon);
			break;
		}
		case EItemType::kConsumable:
		{
			return HealthKitNeedScore(ownerPawn, item);
			break;
		}
		default:
			break;
	}

	return 0.0f;
}

// Compair with owning best item from inventory
float UChooseBestLootTask::ValueScore(const UItemData* item)
{
	float itemValue = item->m_value;

	return FMath::Clamp(itemValue / UItemData::GetMaxItemValue() , 0.0f, 1.0f);
}

float UChooseBestLootTask::DistanceScore(const AActor* item, const AFirstPersonBase* ownerPawn)
{
	FVector pawnLocation = ownerPawn->GetActorLocation();
	FVector itemLocation = item->GetActorLocation();

	float dist = FVector::Dist(itemLocation, pawnLocation);
	return 1.0f - FMath::Clamp(dist / MaxDistanceValue, 0.0f, 1.0f);;
}


//////////////////////////////////
// Need Score Calculations
//////////////////////////////////
float UChooseBestLootTask::WeaponNeedScore(const UWeaponItem* pOwnedWeapon, const UWeaponItem* pNewWeapon)
{
	if (!IsValid(pNewWeapon))
		return 0.0f;

	// no weapons, give highest score
	if (!IsValid(pOwnedWeapon))
		return 1.0f;

	// same weapons, no need score
	if (pOwnedWeapon->GetDamage() >= pNewWeapon->GetDamage())
		return 0.0f;

	return FMath::Clamp((pNewWeapon->GetDamage() - pOwnedWeapon->GetDamage()) / UWeaponItem::GetMaxDamage(), 0.0f, 1.0f);
}

float UChooseBestLootTask::AmmoNeedScore(const AFirstPersonBase* ownerPawn, const UItemData* item) const
{
	FInventorySlot weaponSlot = ownerPawn->GetEquipSlot(EEquipType::kWeapon);
	const UWeaponItem* weapon = Cast<UWeaponItem>(weaponSlot.GetItem());

	int32 ammoCount = ownerPawn->GetAllOwningItemCount(item);

	// no weapons, no need score
	if (!IsValid(weapon))
	{
		return 0.0f;
	}

	ammoCount += weaponSlot.GetQuantity();
	int32 maxAmmoNeed = weapon->GetMaxAmmoScore();

	return 1.0f - FMath::Clamp(static_cast<float>(ammoCount) / static_cast<float>(maxAmmoNeed), 0.0f, 1.0f);
}

float UChooseBestLootTask::HealthKitNeedScore(const AFirstPersonBase* ownerPawn, const UItemData* item) const
{
	int healkitCount = ownerPawn->GetAllOwningItemCount(item);

	return 1.0f - FMath::Clamp(static_cast<float>(healkitCount) / static_cast<float>(kMaxHealkitCount), 0.0f, 1.0f);;
}
