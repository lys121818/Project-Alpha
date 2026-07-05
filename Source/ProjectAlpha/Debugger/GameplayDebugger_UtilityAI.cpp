// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayDebugger_UtilityAI.h"
#include "EnemyAIController.h"
#include "EnemyAi.h"
#include "UtilityDebugScore.h"

GameplayDebugger_UtilityAI::GameplayDebugger_UtilityAI()
{
    SetDataPackReplication(&Data);
}

void GameplayDebugger_UtilityAI::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
    Data.Text.Empty();

    APawn* debugPawn = Cast<APawn>(DebugActor);
    if (!IsValid(debugPawn))
        return;

    AEnemyAIController* pController = Cast<AEnemyAIController>(debugPawn->GetController());
    if (!IsValid(pController))
        return;

    const TArray<FUtilityDebugScore>& scores = pController->GetLastDebugScores();

    Data.Text += TEXT("{yellow]Utility AI\n");

    Data.Text += TEXT("{blue}[Selected Action]:");
    Data.Text += FString::Printf(TEXT("{blue} %s\n"), *pController->GetLastSelectedActionName().ToString());

    // decisions
    for (const auto& decisionScore : scores)
    {
        const FString BlockedText = decisionScore.GetIsBlockedByRequired() ? TEXT("{red}[BLOCKED]") : TEXT("");
        const FString RemovedText = decisionScore.GetScore() < pController->GetLastCutoffScore() ? TEXT("{red}[Removed] ") : TEXT("");

        if(decisionScore.GetIsBlockedByRequired())
            Data.Text += FString::Printf(TEXT("{white}%s: %s - %s\n"), *decisionScore.GetDecisionName().ToString(),*BlockedText, *decisionScore.GetBlockedRequiredName().ToString());
        else
            Data.Text += FString::Printf(TEXT("{white}%s: %f %s\n"), *decisionScore.GetDecisionName().ToString(), decisionScore.GetScore(), *RemovedText);



        // considerations
        for (const FConsiderationDebugScore& considerationScore : decisionScore.GetConsiderationScores())
        {
            Data.Text += FString::Printf(TEXT("  {gray} - %s \n\tRaw Score: %f \n\tWeighted Score: %f\n"),
                *considerationScore.ConsiderationName.ToString(), considerationScore.RawScore, considerationScore.WeightedScore);
        }

    }
}

void GameplayDebugger_UtilityAI::DrawData(APlayerController * OwnerPC, FGameplayDebuggerCanvasContext & CanvasContext)
{
    if (!Data.Text.IsEmpty())
    {
        CanvasContext.Printf(TEXT("%s"), *Data.Text);
    }
}

TSharedRef<FGameplayDebuggerCategory> GameplayDebugger_UtilityAI::MakeInstance()
{
    return MakeShareable(new GameplayDebugger_UtilityAI());
}
