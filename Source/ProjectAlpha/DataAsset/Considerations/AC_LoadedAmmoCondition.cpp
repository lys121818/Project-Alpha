// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_LoadedAmmoCondition.h"
#include "AIUtilityContext.h"
#include "EnemyAIController.h"
#include "FirstPersonBase.h"
#include "WeaponItem.h"
#include "InventorySlot.h"

float UAC_LoadedAmmoCondition::RawScore(const FAIUtilityContext& context) const
{

    AFirstPersonBase* pOwnerPawn = context.OwnerPawn;

    if (!IsValid(pOwnerPawn))
        return 0.0f;

    FInventorySlot pWeaponSlot = pOwnerPawn->GetEquipSlot(EEquipType::kWeapon);
    const UWeaponItem* pWeapon = Cast<UWeaponItem>(pWeaponSlot.GetItem());

    if (!IsValid(pWeapon))
        return 0.0f;

    float loadedAmmoRatio = static_cast<float>(pWeaponSlot.GetQuantity()) / static_cast<float>(pWeapon->GetMaxAmmoCount());

    return FMath::Clamp(loadedAmmoRatio,0.0f,1.0f);
}
