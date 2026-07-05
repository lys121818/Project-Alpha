// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_WeaponCondition.h"
#include "AIUtilityContext.h"
#include "../TargetData.h"
#include "FirstPersonBase.h"
#include "WeaponItem.h"

float UAC_WeaponCondition::RawScore(const FAIUtilityContext& context) const
{
    AFirstPersonBase* pawn = context.OwnerPawn;

    if (!IsValid(pawn))
        return 0.0f;

    UWeaponItem* weapon = Cast<UWeaponItem>(pawn->GetEquipSlot(EEquipType::kWeapon).GetItem());

    if (!IsValid(weapon))
        return 0.0f;

    const float score = FMath::Clamp(weapon->GetDamage() / MaxDamageScore, 0.0f, 1.0f);

    return score;
}