// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTALPHA_API UWeaponItem : public UItemData
{
	GENERATED_BODY()

	static constexpr float kMaxDamage = 30.0f;
	static constexpr int kMaxReloadCount = 5;	// max ammo reload count

public:

	static float GetMaxDamage() { return kMaxDamage; }
	int32 GetMaxAmmoScore() const { return MaxAmmoCount * kMaxReloadCount; }

	virtual void UseItem(AFirstPersonBase* pUserPawn) override;

	const TObjectPtr<UItemData>& GetUsingAmmo() const { return UsingAmmo; }

	float GetDurationBetweenFire() const { return DurationBetweenFire; }

	int32 GetMaxAmmoCount() const { return MaxAmmoCount; }
	float GetDamage() const { return Damage; }
	int32 GetCurrentAmmoCount() const { return CurrentAmmoCount; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UItemData> UsingAmmo = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxAmmoCount = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 CurrentAmmoCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DurationBetweenFire = 0.0f;
};
