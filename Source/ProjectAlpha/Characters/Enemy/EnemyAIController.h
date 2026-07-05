// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "TargetData.h"
#include "UtilityDebugScore.h"
#include "EnemyAIController.generated.h"

class UAiDecision;
/**
 * 
 */
UCLASS()
class PROJECTALPHA_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	static constexpr float kSightRadius = 5000.0f;
	static constexpr float kLoseSightRadius = kSightRadius + 500.0f;
	static constexpr float kVisionAngle = 165.0f;
	static constexpr float kExpireCauserTimer = 5.0f;
	static constexpr float kExpireItemFoundTimer = 30.0f;
	static constexpr float kDecisionQueueTime = 5.0f;

public:
	AEnemyAIController();

	void AddCausedTarget(AActor* target);

	FVector GetLastKnownTargetLocation() { return m_targetData.GetLastKnownTargetLocation(); }

	const TArray<FDamagedActor>& GetCausedTargets() { return m_targetData.GetCausedTargets(); }
	const TArray<TWeakObjectPtr<AActor>>& GetVisibleTargets() { return m_targetData.GetVisibleTargets(); }

	const FTargetData* GetTargetData() { return &m_targetData; }

	UFUNCTION()
	bool IsReadyForDecisionQueue() { return m_isReadyToAddQueue; }

	UFUNCTION()
	bool IsInCombat();

	UFUNCTION()
	void AddToDecisionQueue();


	// Debugs
	UFUNCTION()
	void AddLastDebugScores(FUtilityDebugScore debugScore);
	UFUNCTION()
	void ClearLastDebugScores();

	void SetLastSelectedAction(FName action) { LastSelectedAction = action; }

	const TArray<FUtilityDebugScore>& GetLastDebugScores() const { return LastUtilityScores; }
	UFUNCTION()
	void SetLastCutoffScore(float score);
	float GetLastCutoffScore() { return LastCutoffScore; }
	const FName& GetLastSelectedActionName() const { return LastSelectedAction; }

	void SetRegionVisited(ARegionArea* region);

	const TMap<TWeakObjectPtr<ARegionArea>, FAreaMemory>& GetKnownAreas() const { return KnownAreas; }

	TObjectPtr<ARegionArea> GetBestSearchRegion(const TArray<TObjectPtr<ARegionArea>>& connectedAreas);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnTargetPerceptionUpdate(AActor* actor, FAIStimulus stimulus);

	void UpdateVisibleTargets();

	void UpdateLastKnownEnemyLocation(AActor* enemy, ARegionArea* newArea);

	void CleanupKnownAreas();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Perception")
	TObjectPtr<UAIPerceptionComponent> m_pPerceptionCmp;

private:
	FTargetData m_targetData;
	float m_enqueueTimer = kDecisionQueueTime;
	bool m_isReadyToAddQueue = false;


	UPROPERTY(EditAnywhere, Category = "RegionMemory")
	TMap<TWeakObjectPtr<ARegionArea>, FAreaMemory> KnownAreas;

	UPROPERTY(EditAnywhere, Category = "RegionMemory")
	TMap<TWeakObjectPtr<AActor>, TWeakObjectPtr<ARegionArea>> EnemyLastKnownArea;

	UPROPERTY()
	TArray<FUtilityDebugScore> LastUtilityScores;

	UPROPERTY()
	FName LastSelectedAction;

	UPROPERTY()
	float LastCutoffScore = 0.0f;


};
