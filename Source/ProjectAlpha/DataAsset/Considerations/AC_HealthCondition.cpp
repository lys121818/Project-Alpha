// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_HealthCondition.h"
#include "AIUtilityContext.h"
#include "FirstPersonBase.h"

float UAC_HealthCondition::RawScore(const FAIUtilityContext& context) const
{
	AFirstPersonBase* pOwnerPawn = context.OwnerPawn;

	if (!IsValid(pOwnerPawn))
		return 0.0f;

	float healthRatio = pOwnerPawn->GetHealth() / pOwnerPawn->GetMaxHealth();


	return FMath::Clamp(healthRatio, 0.0f, 1.0f);
}
