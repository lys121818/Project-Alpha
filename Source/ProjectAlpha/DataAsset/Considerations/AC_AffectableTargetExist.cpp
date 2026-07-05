// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_AffectableTargetExist.h"
#include "AIUtilityContext.h"
#include "../TargetData.h"

float UAC_AffectableTargetExist::RawScore(const FAIUtilityContext& context) const
{
    const TArray<TWeakObjectPtr<AActor>>& visibleTargets = context.TargetData->GetVisibleTargets();
    const TArray<FDamagedActor>& causedTargets = context.TargetData->GetCausedTargets();

    return (visibleTargets.IsEmpty() && causedTargets.IsEmpty() )? 0.0f : 1.0f;
}