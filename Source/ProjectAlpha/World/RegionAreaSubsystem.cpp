// Fill out your copyright notice in the Description page of Project Settings.


#include "RegionAreaSubsystem.h"
#include "RegionArea.h"
#include "EnemyAIController.h"

void URegionAreaSubsystem::RegisterRegion(ARegionArea* region)
{
    Regions.AddUnique(region);
}

void URegionAreaSubsystem::UnregisterRegion(ARegionArea* region)
{
    Regions.Remove(region);
}


ARegionArea* URegionAreaSubsystem::FindRegionForActor(AActor* actor) const
{
    for (const auto& region : Regions)
    {
        if (!IsValid(region))
            continue;

        if (region->IsInsideRegionArea(actor))
        {
            return region.Get();
        }
    }

    return nullptr;
}

ARegionArea* URegionAreaSubsystem::FindRegionInLocation(FVector location) const
{
    for (const auto& region : Regions)
    {
        if (!IsValid(region))
            continue;

        if (region->IsInsideRegionArea(location))
            return region.Get();
    }

    return nullptr;
}

float URegionAreaSubsystem::GetAreaDangerScore(const AEnemyAIController* controller, ARegionArea* targetRegion)
{
    const TMap<TWeakObjectPtr<ARegionArea>, FAreaMemory>& knownAreas = controller->GetKnownAreas();

    float total = 0.0f;
    int32 count = 0;
    // collect all known danger areas
    for (const auto& area : knownAreas)
    {
        const TWeakObjectPtr<ARegionArea>& region = area.Key;
        const FAreaMemory& memory = area.Value;

        float dangerScore = memory.GetDangerScore(GetWorld()->GetTimeSeconds());

        if (dangerScore <= 0.0f)
            continue;

        // BFS connected regions to reach target region for all danger areas and sub total score
        int32 distance = GetRegionDistance(region, targetRegion);

        if (distance < 0)
            continue;

        total += dangerScore * FMath::Pow(0.5f, distance);
        count++;
    }

    if (count <= 0)
        return 0;

    return (total /= count);

}

int32 URegionAreaSubsystem::GetRegionDistance(const TWeakObjectPtr<ARegionArea>& dangerArea, const TWeakObjectPtr<ARegionArea>& targetArea)
{
    if (!dangerArea.IsValid() || !targetArea.IsValid())
        return -1;

    if (dangerArea == targetArea)
        return 0;

    TQueue<TPair<TWeakObjectPtr<ARegionArea>, int32>> queue;    // pair of region and distance
    TSet<TWeakObjectPtr<ARegionArea>> visited;  // visited area list

    queue.Enqueue({ dangerArea, 0 });
    visited.Add(dangerArea);

    while (!queue.IsEmpty())
    {
        TPair<TWeakObjectPtr<ARegionArea>, int32> current;
        queue.Dequeue(current);

        if (!current.Key.IsValid())
            continue;

        const int32 distance = current.Value;

        // loop all unvisited connecting areas
        for (const TObjectPtr<ARegionArea>& connectedArea : current.Key->GetConnectedAreas())
        {
            if (!IsValid(connectedArea))
                continue;

            TWeakObjectPtr<ARegionArea> area = connectedArea.Get();

            if (visited.Contains(area))
                continue;

            // found
            if (area == targetArea)
                return distance + 1;

            // enqueue neighbor area node to search
            visited.Add(area);
            queue.Enqueue({ area, distance + 1 });
        }
    }

    // not connected to target area
    return -1;
}
