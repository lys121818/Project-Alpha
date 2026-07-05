// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EquipSlotComponent.h"
#include "ItemData.h"

// Sets default values for this component's properties
UEquipSlotComponent::UEquipSlotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	// set 0 meaning nothing is equipped

	InitEquipSlots();
}

// Actor's to know that before quipping item if the return value is true
// Actor's need to uneqquip the item and return it to their inventory
bool UEquipSlotComponent::EquipItem(UItemData* item)
{
	// check type of the item
	EEquipType type = item->m_equipType;
	if (type == EEquipType::kNone)
		return false;

	m_equipSlots[type].AddItem(item);

	OnEquipItem.Broadcast();

	return true;
}

// Same as equipping, it's Actor's job to work with the removed item
bool UEquipSlotComponent::UnEquipItem(UItemData* item)
{
	EEquipType type = item->m_equipType;
	if (m_equipSlots[type].GetQuantity() > 0)
	{
		m_equipSlots[type].Reset();
		OnUnequipItem.Broadcast(item);
		return true;
	}

	return false;
}


// Called when the game starts
void UEquipSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UEquipSlotComponent::InitEquipSlots()
{
	// set add all the slots empty
	for (size_t i = 0; i < static_cast<int>(EEquipType::kNone); ++i)
	{
		m_equipSlots.Add(static_cast<EEquipType>(i));
	}
}

bool UEquipSlotComponent::IsSlotEmpty(const EEquipType& type) const
{
	if (type == EEquipType::kNone)
		return false;

	if (m_equipSlots[type].GetQuantity() <= 0)
	{
		return true;
	}
	return false;
}

// Called every frame
void UEquipSlotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

