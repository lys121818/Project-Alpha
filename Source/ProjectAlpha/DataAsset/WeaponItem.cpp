// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItem.h"
#include "FirstPersonBase.h"

void UWeaponItem::UseItem(AFirstPersonBase* pUserPawn)
{
    if (!IsValid(pUserPawn))
        return;

    pUserPawn->EquipItem(this);
}
