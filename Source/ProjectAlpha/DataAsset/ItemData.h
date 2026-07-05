// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EquipSlotComponent.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	kAmmo,
	kEquippable,
	kConsumable,
	kMisc
};

class UStaticMeshComponent;
class UTexture2D;

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTALPHA_API UItemData : public UDataAsset
{
	GENERATED_BODY()
	static constexpr float kMaxItemValue = 5000.0f;
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName m_name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemType m_itemType = EItemType::kMisc;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipType m_equipType = EEquipType::kNone;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool m_isStackable = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 m_count = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float m_value = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
	int32 m_maxStack = 99;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> m_icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> m_staticMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	FVector m_worldScale = FVector(1.0f);

	static float GetMaxItemValue() { return kMaxItemValue; }

	UFUNCTION()
	float GetItemValue() { return m_value * m_count; }

	UFUNCTION()
	virtual void UseItem(AFirstPersonBase* pUserPawn) PURE_VIRTUAL(UItemData::UseItem, );
};
