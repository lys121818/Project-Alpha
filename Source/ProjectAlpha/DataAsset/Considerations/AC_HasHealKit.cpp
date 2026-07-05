// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_HasHealKit.h"
#include "AIUtilityContext.h"
#include "TargetData.h"
#include "EnemyAIController.h"
#include "FirstPersonBase.h"
#include "BehaviorTree/BlackboardComponent.h"

float UAC_HasHealKit::RawScore(const FAIUtilityContext& context) const
{
    AFirstPersonBase* pOwnerPawn = context.OwnerPawn;

    if (!IsValid(pOwnerPawn))
        return 0.0f;

    return pOwnerPawn->GetTotalHealKitCount() > 0 ? 1.0f : 0.0f;
}
