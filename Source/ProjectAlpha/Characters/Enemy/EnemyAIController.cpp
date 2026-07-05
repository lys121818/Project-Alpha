// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "../FirstpersonBase.h"
#include "PickupItem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAi.h"
#include "UtilityAI.h"
#include "UtilityDebugScore.h"
#include "RegionAreaSubsystem.h"
#include "RegionArea.h"

AEnemyAIController::AEnemyAIController()
{
	m_pPerceptionCmp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionCmp"));

	// set sight sense
	UAISenseConfig_Sight* sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Sense"));
	sight->SightRadius = kSightRadius;
	sight->LoseSightRadius = kLoseSightRadius;
	sight->PeripheralVisionAngleDegrees = kVisionAngle / 2.0f;

	sight->DetectionByAffiliation.bDetectEnemies = true;
	sight->DetectionByAffiliation.bDetectNeutrals = true;
	sight->DetectionByAffiliation.bDetectFriendlies = true;

	// add sense to component
	m_pPerceptionCmp->ConfigureSense(*sight);
	m_pPerceptionCmp->SetDominantSense(UAISenseConfig_Sight::StaticClass());

	// events function connected with sense
	m_pPerceptionCmp->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdate);

}

void AEnemyAIController::AddCausedTarget(AActor* target)
{
	m_targetData.AddCausedTarget(target, GetWorld()->GetTimeSeconds());
}


bool AEnemyAIController::IsInCombat()
{
	AFirstPersonBase* ownerPawn = Cast<AFirstPersonBase>(GetPawn());

	return ownerPawn->IsInCombat();
}

void AEnemyAIController::AddToDecisionQueue()
{
	AEnemyAi* ownerPawn = Cast<AEnemyAi>(GetPawn());

	if (!IsValid(ownerPawn))
		return;


	const TArray<TObjectPtr<UAiDecision>>& decisions = ownerPawn->GetDecisions();

	if (decisions.IsEmpty())
		return;

	UUtilityAI* utility = GetWorld()->GetSubsystem<UUtilityAI>();

	utility->AddToDecisionQueue(this);

	m_isReadyToAddQueue = false;
	m_enqueueTimer = kDecisionQueueTime;
}

void AEnemyAIController::AddLastDebugScores(FUtilityDebugScore debugScore)
{
	LastUtilityScores.Add(debugScore);
}

void AEnemyAIController::ClearLastDebugScores()
{
	LastUtilityScores.Empty();
}

void AEnemyAIController::SetLastCutoffScore(float score)
{
	LastCutoffScore = score;
}

void AEnemyAIController::SetRegionVisited(ARegionArea* region)
{
	KnownAreas.FindOrAdd(region).SetLastVisitedTime(GetWorld()->GetTimeSeconds());
}

TObjectPtr<ARegionArea> AEnemyAIController::GetBestSearchRegion(const TArray<TObjectPtr<ARegionArea>>& connectedAreas)
{
	URegionAreaSubsystem* pRegionAreaSubsystem = GetWorld()->GetSubsystem<URegionAreaSubsystem>();

	TArray<ARegionArea*> bestAreas;
	float best = FLT_MIN;
	for (const auto& area : connectedAreas)
	{
		float score = 0.0f;
		
		// visited score
		if (!KnownAreas.Contains(area))
		{
			score += 1.0f;
		}
		else
		{
			score += KnownAreas[area].GetVisitedScore(GetWorld()->GetTimeSeconds());
		}

		// danger score
		score += (1.0f - pRegionAreaSubsystem->GetAreaDangerScore(this, area));

		if (score > best)
		{
			best = score;
			bestAreas.Reset();
			bestAreas.Add(area);
		}
		// close to best
		else if (FMath::Abs(score - best) <= 0.05)
		{
			bestAreas.Add(area);
		}
	}

	int32 randomPick = FMath::RandRange(0, bestAreas.Num() - 1);

	return bestAreas[randomPick];
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	AddToDecisionQueue();
	m_enqueueTimer = kDecisionQueueTime;
	m_isReadyToAddQueue = false;
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_enqueueTimer -= DeltaTime;

	if (m_enqueueTimer <= 0)
		m_isReadyToAddQueue = true;

	// remove expired caused actors
	m_targetData.RemoveCausedTargets(GetWorld()->GetTimeSeconds(), kExpireCauserTimer);
	m_targetData.RemoveFoundItem(GetWorld()->GetTimeSeconds(), kExpireItemFoundTimer);
	CleanupKnownAreas();
	UpdateVisibleTargets();
}

void AEnemyAIController::OnTargetPerceptionUpdate(AActor* actor, FAIStimulus stimulus)
{
	check(actor)

	if (!IsValid(actor))
		return;
	AFirstPersonBase* character = Cast<AFirstPersonBase>(actor);

	///////////////////////////////////////////////
	// target actor
	///////////////////////////////////////////////
	if (IsValid(character))
	{
		if (stimulus.WasSuccessfullySensed())
		{
			m_targetData.AddVisibleTarget(actor);
		}
		else
		{
			m_targetData.RemoveVisibleTarget(actor);
			m_targetData.SetLastKnownTargetLocation(stimulus.StimulusLocation);
		}
	}

	APickupItem* pickup = Cast<APickupItem>(actor);

	///////////////////////////////////////////////
	// item
	///////////////////////////////////////////////
	if (IsValid(pickup))
	{
		if (stimulus.WasSuccessfullySensed())
		{
			m_targetData.AddFoundItem(pickup,GetWorld()->GetTimeSeconds());

			// add to memory
			URegionAreaSubsystem* regionSubsystem = GetWorld()->GetSubsystem<URegionAreaSubsystem>();
			ARegionArea* region = regionSubsystem->FindRegionForActor(pickup);

			if (!IsValid(region))
				return;

			FAreaMemory& memory = KnownAreas.FindOrAdd(region);
			memory.AddKnownItem(actor);
		}
	}
}


void AEnemyAIController::UpdateVisibleTargets()
{
	URegionAreaSubsystem* regionSubsystem = GetWorld()->GetSubsystem<URegionAreaSubsystem>();


	const TArray<TWeakObjectPtr<AActor>>& visibleTargets = m_targetData.GetVisibleTargets();

	for (const auto& target : visibleTargets)
	{
		if (!IsValid(target.Get()))
			continue;

		ARegionArea* locatedArea = regionSubsystem->FindRegionForActor(target.Get());

		if (!IsValid(locatedArea))
			continue;

		UpdateLastKnownEnemyLocation(target.Get(), locatedArea);
	}
}

void AEnemyAIController::UpdateLastKnownEnemyLocation(AActor* enemy, ARegionArea* newArea)
{
	if (!IsValid(enemy) || !IsValid(newArea))
		return;

	// Remove existing old data
	if (TWeakObjectPtr<ARegionArea>* oldArea = EnemyLastKnownArea.Find(enemy))
	{
		if (IsValid(oldArea->Get()) && oldArea->Get() != newArea)
		{
			FAreaMemory* oldMemory = KnownAreas.Find(oldArea->Get());

			if (oldMemory)
				oldMemory->RemoveEnemy(enemy);
		}
	}

	// Update new data
	FAreaMemory& newMemory = KnownAreas.FindOrAdd(newArea);
	newMemory.AddKnownEnemy(enemy, GetWorld()->GetTimeSeconds());

	EnemyLastKnownArea.Add(enemy, newArea);

}

void AEnemyAIController::CleanupKnownAreas()
{
	TArray<TWeakObjectPtr<ARegionArea>> removeAreaList;
	for (auto& area : KnownAreas)
	{
		if (!area.Key.IsValid())
		{
			removeAreaList.Add(area.Key);
		}

		area.Value.CleanupInValidEnemies();
		area.Value.CleanupInValidItems();

	}

	for (const auto& item : removeAreaList)
	{
		KnownAreas.Remove(item);
	}

	TArray<TWeakObjectPtr<AActor>> removeEnemyList;

	for (auto& enemyArea : EnemyLastKnownArea)
	{
		if (!enemyArea.Key.IsValid() || !enemyArea.Value.IsValid())
		{
			removeEnemyList.Add(enemyArea.Key);
		}
	}

	for (const auto& enemy : removeEnemyList)
	{
		EnemyLastKnownArea.Remove(enemy);
	}
}
