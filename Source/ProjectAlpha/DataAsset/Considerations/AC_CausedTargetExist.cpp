// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_CausedTargetExist.h"
#include "AIUtilityContext.h"
#include "../TargetData.h"

float UAC_CausedTargetExist::RawScore(const FAIUtilityContext& context) const
{
    const TArray<FDamagedActor>& causedTargets = context.TargetData->GetCausedTargets();
    return causedTargets.IsEmpty() ? 0.0f : 1.0f;
}