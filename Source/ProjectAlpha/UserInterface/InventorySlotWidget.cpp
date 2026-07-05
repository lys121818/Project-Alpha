// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "InventorySlot.h"
#include "ItemData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventorySlotWidget::SetSlotData(const FInventorySlot& slotData, int slotIndex)
{
	m_index = slotIndex;

	// show icons
	if (ItemIcon && slotData.GetItem() && slotData.GetItem()->m_icon)
	{
		ItemIcon->SetBrushFromTexture(slotData.GetItem()->m_icon);
		ItemIcon->SetOpacity(1.0f);
	}
	else
	{
		ItemIcon->SetBrushFromTexture(nullptr);
		ItemIcon->SetOpacity(0.0f);
	}

	// item count text
	if (ItemCount)
	{
		if (slotData.GetQuantity() > 1)
		{
			ItemCount->SetText(FText::AsNumber(slotData.GetQuantity()));
		}
		else
		{
			ItemCount->SetText(FText::GetEmpty());
		}
	}

}
