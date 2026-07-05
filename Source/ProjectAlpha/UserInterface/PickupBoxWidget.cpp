// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBoxWidget.h"
#include "InventoryComponent.h"
#include "ItemData.h"

// =============================================
// Public Callable Functions
// =============================================

void UPickupBoxWidget::Init(UInventoryComponent* playerInventory, UInventoryComponent* boxInventory)
{
	PlayerInventory = playerInventory;
	BoxInventory = boxInventory;
}

void UPickupBoxWidget::TransferItemToPlayer(int slotIndex)
{
	if (!PlayerInventory || !BoxInventory)
		return;

	const TArray<FInventorySlot>& slots = BoxInventory->GetSlots();
	if (!slots.IsValidIndex(slotIndex))
		return;

	UItemData* item = slots[slotIndex].GetItem();
	const int32 quantity = slots[slotIndex].GetQuantity();

	if (!item)
		return;

	if (PlayerInventory->AddItem(item, quantity))
		BoxInventory->RemoveItem(item, quantity);
}

void UPickupBoxWidget::TransferItemToBox(int slotIndex)
{
	if (!PlayerInventory || !BoxInventory)
		return;

	const TArray<FInventorySlot>& slots = PlayerInventory->GetSlots();
	if (!slots.IsValidIndex(slotIndex))
		return;

	UItemData* item = slots[slotIndex].GetItem();
	const int32 quantity = slots[slotIndex].GetQuantity();

	if (!item)
		return;

	if (BoxInventory->AddItem(item, quantity))
		PlayerInventory->RemoveItem(item, quantity);
}
