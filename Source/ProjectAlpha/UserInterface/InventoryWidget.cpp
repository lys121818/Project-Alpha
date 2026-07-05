// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InventoryComponent.h"
#include "Components/WrapBox.h"
#include "InventorySlotWidget.h"

void UInventoryWidget::Init(UInventoryComponent* inventoryCmp)
{
	InventoryComponent = inventoryCmp;
	UpdateInventory();
}


void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateInventory();
}

void UInventoryWidget::UpdateInventory()
{
	if (!InventoryComponent || !SlotBox || !SlotWidgetClass)
		return;

	SlotBox->ClearChildren();

	const TArray<FInventorySlot>& slots = InventoryComponent->GetSlots();

	for (int i = 0; i < slots.Num(); ++i)
	{
		// Create user widget
		UInventorySlotWidget* slotWidget = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);

		if (!slotWidget)
			continue;

		slotWidget->SetSlotData(slots[i], i);
		SlotBox->AddChild(slotWidget);
	}
}