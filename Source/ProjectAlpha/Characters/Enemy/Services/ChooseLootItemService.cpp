// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseLootItemService.h"
#include "FirstPersonBase.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TargetData.h"
#include "PickupItem.h"
#include "ItemData.h"
#include "WeaponItem.h"
#include "RandomPickupItem.h"
#include "RegionAreaSubsystem.h"

UChooseLootItemService::UChooseLootItemService()
{
    NodeName = TEXT("Choose Loot Item");

    bNotifyTick = true;
}

void UChooseLootItemService::TickNode(UBehaviorTreeComponent& ownerCmp, uint8* nodeMemory, float deltaSecond)
{
	AEnemyAIController* pController = Cast<AEnemyAIController>(ownerCmp.GetAIOwner());

	if (!IsValid(pController))
		return;

	AFirstPersonBase* pOwnerPawn = Cast<AFirstPersonBase>(pController->GetPawn());

	if (!IsValid(pOwnerPawn))
		return;

	UBlackboardComponent* pBoard = ownerCmp.GetBlackboardComponent();

	if (!IsValid(pBoard))
		return;

	const FName targetItemKey = TEXT("TargetItem");

	const FTargetData* targetData = pController->GetTargetData();
	if (!targetData)
	{
		pBoard->ClearValue(targetItemKey);
		return;
	}

	const TArray<FFoundItem>& foundItems = targetData->GetFoundItems();
	APickupItem* bestItem = nullptr;
	float bestScore = TNumericLimits<float>::Lowest();

	const float totalWeight = DangerWeight + DistanceWeight + ValueWeight;
	if (totalWeight <= 0)
	{
		pBoard->ClearValue(targetItemKey);
		return;
	}

	// find best item
	for (const auto& item : foundItems)
	{
		float currentScore = 0.0f;

		APickupItem* pPickup = Cast<APickupItem>(item.Item);

		if (!IsValid(pPickup))
			continue;

		const UItemData* itemData = pPickup->GetItemData();

		if (!IsValid(itemData))
			continue;


		const float needScore = FMath::Clamp(NeedScore(itemData, pOwnerPawn), 0.0f, 1.0f);
		const float distanceScore = DistanceScore(pPickup, pOwnerPawn);

		if (distanceScore <= 0.0f || needScore <= 0.0f)
			continue;

		const float valueScore = FMath::Clamp(ValueScore(itemData), 0.0f, 1.0f);
		const float safeScore = 1.0f - DangerScore(pPickup, pController);

		// weighted score based on need score
		currentScore = (distanceScore * DistanceWeight) + (valueScore * ValueWeight) + (safeScore * DangerWeight);
		currentScore /= totalWeight;
		currentScore *= needScore;


		if (bestScore < currentScore)
		{
			bestScore = currentScore;
			bestItem = pPickup;
		}
	}

	if (!IsValid(bestItem))
	{
		pBoard->ClearValue(targetItemKey);
		return;
	}

	pBoard->SetValueAsObject(targetItemKey, bestItem);
	return;


}

float UChooseLootItemService::NeedScore(const UItemData* item, const AFirstPersonBase* ownerPawn)
{
	if (!IsValid(item) || !IsValid(ownerPawn))
		return 0.0f;

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

float UChooseLootItemService::ValueScore(const UItemData* item)
{
	float itemValue = item->m_value;

	return FMath::Clamp(itemValue / UItemData::GetMaxItemValue(), 0.0f, 1.0f);
}

float UChooseLootItemService::DistanceScore(const AActor* item, const AFirstPersonBase* ownerPawn)
{
	FVector pawnLocation = ownerPawn->GetActorLocation();
	FVector itemLocation = item->GetActorLocation();

	float dist = FVector::Dist(itemLocation, pawnLocation);
	return 1.0f - FMath::Clamp(dist / MaxDistanceValue, 0.0f, 1.0f);
}

float UChooseLootItemService::DangerScore(const AActor* item, const AEnemyAIController* controller)
{
	URegionAreaSubsystem* regionSubsystem = GetWorld()->GetSubsystem<URegionAreaSubsystem>();

	if (!IsValid(regionSubsystem))
		return 0.0f;


	ARegionArea* area = regionSubsystem->FindRegionInLocation(item->GetActorLocation());

	if (!IsValid(area))
		return 0.0f;

	return FMath::Clamp(regionSubsystem->GetAreaDangerScore(controller, area), 0.0f, 1.0f);
}

float UChooseLootItemService::WeaponNeedScore(const UWeaponItem* pOwnedWeapon, const UWeaponItem* pNewWeapon)
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

float UChooseLootItemService::AmmoNeedScore(const AFirstPersonBase* ownerPawn, const UItemData* item) const
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

float UChooseLootItemService::HealthKitNeedScore(const AFirstPersonBase* ownerPawn, const UItemData* item) const
{
	const int32 healkitCount = ownerPawn->GetAllOwningItemCount(item);
	const float inventoryPressure = 1.0f - FMath::Clamp(static_cast<float>(healkitCount) / static_cast<float>(kMaxHealkitCount), 0.0f, 1.0f);
	const float hpDeficit = 1.0f - FMath::Clamp(ownerPawn->GetHealth() / ownerPawn->GetMaxHealth(), 0.0f, 1.0f);

	return (inventoryPressure + hpDeficit) * 0.5f;
}
