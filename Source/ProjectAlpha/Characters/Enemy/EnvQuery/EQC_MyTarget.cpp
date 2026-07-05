// Fill out your copyright notice in the Description page of Project Settings.


#include "EQC_MyTarget.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnemyAIController.h"

UEQC_MyTarget::UEQC_MyTarget()
{

}

void UEQC_MyTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData & ContextData) const
{
	AEnemyAIController* controller = Cast<AEnemyAIController>(QueryInstance.Owner);

	if (!controller)
		return;

	APawn* pawn = controller->GetPawn();

	if (!pawn)
		return;


}
