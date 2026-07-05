// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_InCombat.h"
#include "AIUtilityContext.h"
#include "EnemyAIController.h"

float UAC_InCombat::RawScore(const FAIUtilityContext& context) const
{
    AEnemyAIController* controller = context.Controller;

    if (!IsValid(controller))
        return 0.0f;

    return controller->IsInCombat() ? 1.0f : 0.0f;
}
