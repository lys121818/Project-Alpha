// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlot.generated.h"
/**
 * 
 */

class UItemData;

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	void SetItemData(UItemData* item, int amount);
	const TObjectPtr<UItemData>& GetItem() const { return m_item; }
	void AddItem(UItemData* item) { m_item = item; }
	void AddQuantity(int32 amount) { m_quantity += amount; }
	int32 GetQuantity() const { return m_quantity; }
	FName GetName() const { return m_name; }
	void Reset();
	float GetSlotValue() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UItemData> m_item = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName m_name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 m_quantity = 0;

};
