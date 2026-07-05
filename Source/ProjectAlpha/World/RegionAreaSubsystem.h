// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RegionAreaSubsystem.generated.h"

class ARegionArea;
class AEnemyAIController;
/**
 * 
 */
UCLASS()
class PROJECTALPHA_API URegionAreaSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void RegisterRegion(ARegionArea* region);
	void UnregisterRegion(ARegionArea* region);

	ARegionArea* FindRegionForActor(AActor* actor) const;
	ARegionArea* FindRegionInLocation(FVector location) const;

	float GetAreaDangerScore(const AEnemyAIController* controller, ARegionArea* targetRegion);

private:
	int32 GetRegionDistance(const TWeakObjectPtr<ARegionArea>& dangerArea, const TWeakObjectPtr<ARegionArea>& targetArea);

	UPROPERTY()
	TArray<TObjectPtr<ARegionArea>> Regions;


};
