// Fill out your copyright notice in the Description page of Project Settings.


#include "ValidateCoverPointService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectAlphaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyAi.h"
#include "EnemyAiController.h"

UValidateCoverPointService::UValidateCoverPointService()
{
    NodeName = TEXT("Validate Cover Point");
 
    bNotifyTick = true;
}

void UValidateCoverPointService::TickNode(UBehaviorTreeComponent & ownerCmp, uint8 * nodeMemory, float deltaSecond)
{
    Super::TickNode(ownerCmp, nodeMemory, deltaSecond);

    AAIController* pController = ownerCmp.GetAIOwner();

    if (!pController)
        return;

    ACharacter* pOwnerAi = Cast<ACharacter>(pController->GetPawn());
    if (!pOwnerAi)
        return;

    UBlackboardComponent* pBoard = ownerCmp.GetBlackboardComponent();
    if (!pBoard)
        return;


    static const FName kTargetLocationKey = TEXT("CausedTarget");
    static const FName kOnCoverKey = TEXT("OnCover");
    const UObject* target = pBoard->GetValueAsObject(kTargetLocationKey);

    if (!target)
    {
        pBoard->SetValueAsBool(kOnCoverKey, false);
        return;
    }

    const APawn* targetPawn = Cast<APawn>(target);

    if (!targetPawn)
    {
        pBoard->SetValueAsBool(kOnCoverKey, false);
        return;
    }

    FVector TargetViewLocation;
    FRotator TargetViewRotation;

    targetPawn->GetActorEyesViewPoint(TargetViewLocation, TargetViewRotation);

    const FVector ownerLocation = pOwnerAi->GetActorLocation();

    // check if target -> owner is able to attack
    if (IsCoveredFromTarget(TargetViewLocation, ownerLocation, pOwnerAi))
    {
        pBoard->SetValueAsBool(kOnCoverKey, true);
    }
    else
        pBoard->SetValueAsBool(kOnCoverKey, false);
}

bool UValidateCoverPointService::IsCoveredFromTarget(FVector from, FVector to, AActor* owner)
{

    UWorld* pWorld = GetWorld();
    if (!pWorld)
        return false;

    AProjectAlphaGameMode* gameMode = Cast<AProjectAlphaGameMode>(UGameplayStatics::GetGameMode(pWorld));
    if (!gameMode)
        return false;

    return !gameMode->CheckIfTargetVisible(from, to, owner);
}
