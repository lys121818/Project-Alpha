// Fill out your copyright notice in the Description page of Project Settings.


#include "ValidateAttackService.h"
#include "EnemyAi.h"
#include "EnemyAiController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectAlphaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "FirstPersonBase.h"

UValidateAttackService::UValidateAttackService()
{

}

void UValidateAttackService::TickNode(UBehaviorTreeComponent & ownerCmp, uint8 * nodeMemory, float deltaSecond)
{
    Super::TickNode(ownerCmp, nodeMemory, deltaSecond);

    AAIController* pController = ownerCmp.GetAIOwner();

    if (!pController)
        return;

    AEnemyAi* pOwnerAi = Cast<AEnemyAi>(pController->GetPawn());
    if (!pOwnerAi)
        return;

    UBlackboardComponent* pBoard = ownerCmp.GetBlackboardComponent();
    if (!pBoard)
        return;


    static const FName kTargetCharacterKey = TEXT("TargetCharacter");
    static const FName kCanSeeTargetKey = TEXT("CanSeeTarget");
    const UObject* target = pBoard->GetValueAsObject(kTargetCharacterKey);

    if (!IsValid(target))
        return;


    const AFirstPersonBase* targetPawn = Cast<AFirstPersonBase>(target);

    if (!IsValid(targetPawn))
    {
        pBoard->SetValueAsBool(kCanSeeTargetKey, false);
        return;
    }

	// Check for HasWeapon
	// Note: if this is false, it should be an error. Utility Service should not lead to Attack State when HasWeapon is false
    static const FName kHasWeaponKey = TEXT("HasWeapon");

    if (!pOwnerAi->HasWeapon())
    {
        pBoard->SetValueAsBool(kHasWeaponKey, false);
        return;
    }

	// Check for valid attack point
    FVector ownerViewLocation;
    FRotator ownerViewRotation;

    pOwnerAi->GetActorEyesViewPoint(ownerViewLocation, ownerViewRotation);

    const FVector targetLocation = targetPawn->GetActorLocation();
    if (IsTargetVisible(ownerViewLocation, targetPawn->GetActorLocation(), pOwnerAi))
    {
        pBoard->SetValueAsBool(kCanSeeTargetKey, true);
    }
    else
    {
        pBoard->SetValueAsBool(kCanSeeTargetKey, false);
    }


	// Check for Need Reload
}

bool UValidateAttackService::IsTargetVisible(FVector from, FVector to, AActor* owner)
{
    UWorld* pWorld = GetWorld();
    if (!pWorld)
        return false;

    AProjectAlphaGameMode* gameMode = Cast<AProjectAlphaGameMode>(UGameplayStatics::GetGameMode(pWorld));
    if (!gameMode)
        return false;

    return gameMode->CheckIfTargetVisible(from, to, owner);
}
