// Fill out your copyright notice in the Description page of Project Settings.
#include "InventoryComponent.h"
#include "ItemData.h"

static int32 AddToStack(FInventorySlot& slot, int32 amount)
{
	const int32 space = slot.GetItem()->m_maxStack - slot.GetQuantity();

	const int32 add = FMath::Clamp(amount, 0, space);
	slot.AddQuantity(add);
	return amount - add;

}

bool UInventoryComponent::AddItem(UItemData* item, int32 amount)
{
	if (!item || amount <= 0) return false;

	int32 remaining = amount;

	// stack existing
	if (item->m_isStackable)
	{
		for (FInventorySlot& Slot : Slots)
		{
			if (Slot.GetItem() == item && Slot.GetQuantity() < item->m_maxStack)
			{
				remaining = AddToStack(Slot, remaining);
				if (remaining == 0)
				{ 
					OnInventoryChanged.Broadcast(item); 
					return true; 
				}
			}
		}
	}

	// add slots
	while (remaining > 0)
	{
		// no slots left in inventory slots
		if (Slots.Num() >= Capacity)
			break;
	
		FInventorySlot NewSlot;
		NewSlot.AddItem(item);
	
		const int32 Add = item->m_isStackable ? FMath::Min(remaining, item->m_maxStack) : 1;
		NewSlot.AddQuantity(Add);
		
		Slots.Add(NewSlot);
		remaining -= Add;
	
		if (!item->m_isStackable)
			break; // non-stackable: 1 per slot
	}

	const bool isAdded = (remaining != amount);
	if (isAdded) OnPickup.Broadcast(item);
	return isAdded && remaining == 0;
}

bool UInventoryComponent::RemoveItem(UItemData* item, int32 amount)
{
	if (!item || amount <= 0) 
		return false;

	int32 Remaining = amount;

	for (int32 i = Slots.Num() - 1; i >= 0; --i)
	{
		FInventorySlot& Slot = Slots[i];
		if (Slot.GetItem() != item) 
			continue;

		const int32 Take = FMath::Min(Remaining, Slot.GetQuantity());
		Slot.AddQuantity(-Take);
		Remaining -= Take;

		if (Slot.GetQuantity() <= 0)
			Slots.RemoveAt(i);

		if (Remaining == 0) 
			break;
	}

	const bool bRemovedAny = (Remaining != amount);
	if (bRemovedAny) OnInventoryChanged.Broadcast(item);
	return Remaining == 0;
}

int32 UInventoryComponent::GetItemCount(const UItemData* item) const
{
	if (!item) return 0;

	int32 Total = 0;
	for (const FInventorySlot& Slot : Slots)
	{
		if (Slot.GetItem() == item)
			Total += Slot.GetQuantity();
	}
	return Total;
}

