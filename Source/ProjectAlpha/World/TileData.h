// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TileData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_BODY()
	
public:
	FTileData() = default;
	FTileData(FVector location, bool isStatic = false, bool isSpawnPoint = false);

	void SetLocation(FVector location) { m_location = location; }
	FVector GetLocation() { return m_location; }

	bool IsStatic() { return m_isStatic; }
private:
	FVector m_location = FVector(0,0,0);
	
	bool m_isStatic = false;
	bool m_isSpawnPoint = false;
};
