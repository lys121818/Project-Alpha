// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_LootableItemExist.h"
#include "AIUtilityContext.h"
#include "../TargetData.h"

float UAC_LootableItemExist::RawScore(const FAIUtilityContext& context) const
{
    const TArray<FFoundItem>& lootableItems = context.TargetData->GetFoundItems();
    return lootableItems.IsEmpty() ? 0.0f : 1.0f;
}
