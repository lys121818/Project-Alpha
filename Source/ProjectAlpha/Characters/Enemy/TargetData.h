// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegionArea.h"
#include "TargetData.generated.h"

class URegionAreaSubsystem;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDamagedActor
{
    GENERATED_BODY()

    UPROPERTY()
    TWeakObjectPtr<AActor> Actor = nullptr;

    UPROPERTY()
    float LastTime = 0.f;
};

USTRUCT(BlueprintType)
struct FFoundItem
{
    GENERATED_BODY()

    UPROPERTY()
    TWeakObjectPtr<AActor> Item = nullptr;

    UPROPERTY()
    float LastTime = 0.f;
};


USTRUCT(BlueprintType)
struct FTargetData
{
    GENERATED_BODY();

public:
    const TArray<FDamagedActor>& GetCausedTargets() const { return m_causedTargets; }
    const TArray<TWeakObjectPtr<AActor>>& GetVisibleTargets() const { return m_visibleTargets; }
    const TArray<FFoundItem>& GetFoundItems() const { return m_foundPickupItems; }

    FVector GetDestinationLocation() { return m_destinationLocation; }
    FVector GetLastKnownTargetLocation() { return m_lastKnownTargetLocation; }

    void AddVisibleTarget(TWeakObjectPtr<AActor> actor) { m_visibleTargets.AddUnique(actor); }
    void RemoveVisibleTarget(TWeakObjectPtr<AActor> actor) { m_visibleTargets.Remove(actor); }
    
    void AddFoundItem(AActor* actor, float currentTime);
    void RemoveFoundItem(float currentTime, float expireTime);

    void AddCausedTarget(AActor* actor, float currentTime);
    void RemoveCausedTargets(float currentTime, float expireTime);


    void SetDestination(FVector location) { m_destinationLocation = location; }
    void SetLastKnownTargetLocation(FVector location) { m_lastKnownTargetLocation = location; }


private:
    TArray<TWeakObjectPtr<AActor>> m_visibleTargets;

    UPROPERTY()
    TArray<FFoundItem> m_foundPickupItems;

    UPROPERTY()
    TArray<FDamagedActor> m_causedTargets;

    UPROPERTY()
    FVector m_destinationLocation;

    UPROPERTY()
    FVector m_lastKnownTargetLocation;


};
