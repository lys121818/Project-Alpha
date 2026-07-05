// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_InventoryCondition.h"
#include "AIUtilityContext.h"
#include "EnemyAIController.h"
#include "InventoryComponent.h"
#include "FirstPersonBase.h"
#include "ItemData.h"

float UAC_InventoryCondition::RawScore(const FAIUtilityContext& context) const
{
    AFirstPersonBase* pOwnerPawn = context.OwnerPawn;

    if (!IsValid(pOwnerPawn))
        return 0.0f;

    // total inventory weight
    UInventoryComponent* pInventory = context.OwnerPawn->GetInventoryComponent();

    if (!IsValid(pInventory))
        return 0.0f;

    // evaluate score
    const float capacity = static_cast<float>(pInventory->GetCapacity());
    const float filledSlots = static_cast<float>(pInventory->GetFilledSlotCount());

    if (capacity <= 0 || filledSlots <= 0)
        return 0.0f;
    
    return FMath::Clamp(filledSlots / capacity, 0.0f, 1.0f);
}
