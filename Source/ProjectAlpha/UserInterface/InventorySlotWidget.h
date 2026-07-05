// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

struct FInventorySlot;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetSlotData(const FInventorySlot& slotData, int slotIndex);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCount;

private:
	int m_index;
};
