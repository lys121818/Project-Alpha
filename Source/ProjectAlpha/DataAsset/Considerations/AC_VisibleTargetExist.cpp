// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_VisibleTargetExist.h"
#include "AIUtilityContext.h"
#include "../TargetData.h"

float UAC_VisibleTargetExist::RawScore(const FAIUtilityContext& context) const
{
    const TArray<TWeakObjectPtr<AActor>>& visibleTargets = context.TargetData->GetVisibleTargets();
    return visibleTargets.IsEmpty() ? 0.0f : 1.0f;
}
