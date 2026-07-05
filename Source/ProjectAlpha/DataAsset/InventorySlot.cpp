// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/InventorySlot.h"
#include "ItemData.h"

void FInventorySlot::SetItemData(UItemData* item, int amount)
{
	m_item = item;
	m_name = item->m_name;
	m_quantity = amount;
}

void FInventorySlot::Reset()
{
	m_item = nullptr;
	m_name = "Empty";
	m_quantity = 0;
}

float FInventorySlot::GetSlotValue() const
{
	if (m_item == nullptr)
		return 0.0f;

	return m_item->m_value * m_quantity;
}
