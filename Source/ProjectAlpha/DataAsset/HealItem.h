// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/ItemData.h"
#include "HealItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UHealItem : public UItemData
{
	GENERATED_BODY()
	static constexpr int kMaxReloadCount = 5;	// max ammo reload count

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HealAmount = 0.0f;

	float GetHealAmount() { return HealAmount; }

	virtual void UseItem(AFirstPersonBase* pUserPawn) override;

};
