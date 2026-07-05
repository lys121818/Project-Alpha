// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_AmmoCondition.h"
#include "AIUtilityContext.h"
#include "EnemyAIController.h"
#include "FirstPersonBase.h"
#include "WeaponItem.h"

float UAC_AmmoCondition::RawScore(const FAIUtilityContext& context) const
{
    AFirstPersonBase* pOwnerPawn = context.OwnerPawn;

    if (!IsValid(pOwnerPawn))
        return 0.0f;

    if(!pOwnerPawn->HasWeapon())
        return 0.0f;

    int32 fullAmmoCount = FMath::Clamp(pOwnerPawn->GetCurrentAmmoCount() + pOwnerPawn->GetTotalAmmoCount(), 0, MaxAmmoCount);
    
    return FMath::Clamp(static_cast<float>(fullAmmoCount) / static_cast<float>(MaxAmmoCount), 0.0f, 1.0f);
}
