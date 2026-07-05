// Fill out your copyright notice in the Description page of Project Settings.


#include "EQC_TargetCharacter.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "EnemyAi.h"

void UEQC_TargetCharacter::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	// set cuased target as context
	AEnemyAi* enemy = Cast<AEnemyAi>(QueryInstance.Owner);

	if (!enemy)
		return;

	AEnemyAIController* controller = Cast<AEnemyAIController>(enemy->GetController());

	if (!controller)
		return;

	UBlackboardComponent* pBoard = controller->GetBlackboardComponent();

	if (!pBoard)
		return;

	static const FName kTargetCharacterKey = TEXT("TargetCharacter");
	const AActor* target = Cast<AActor>(pBoard->GetValueAsObject(kTargetCharacterKey));

	if (IsValid(target))
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, target);


}
