// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectAlphaGameMode.generated.h"

class AWorldMap;

/**
 *  Simple GameMode for a first person game
 */
UCLASS(abstract)
class AProjectAlphaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectAlphaGameMode();

	virtual void StartPlay() override;

	AWorldMap* GetWorldMap() const { return m_worldMap; }
	void SetWorldMap(AWorldMap* worldMap) { m_worldMap = worldMap; }

	bool CheckIfTargetVisible(FVector start, FVector end, AActor* ignore);
	bool CheckIfCoverFromTarget(FVector start, FVector end);

private:
	bool CheckRaycastHit(FHitResult& result, FVector start, FVector end, AActor* ignore);

	void AddCharacterToList(AActor* actor);

	UPROPERTY()
	TObjectPtr<AWorldMap> m_worldMap = nullptr;

	UPROPERTY()
	TArray<AActor*> m_worldActors;
};



