// Fill out your copyright notice in the Description page of Project Settings.


#include "HealItem.h"
#include "FirstPersonBase.h"

void UHealItem::UseItem(AFirstPersonBase* pUserPawn)
{
    if (!IsValid(pUserPawn))
        return;

    pUserPawn->HealHealth(HealAmount);
}
