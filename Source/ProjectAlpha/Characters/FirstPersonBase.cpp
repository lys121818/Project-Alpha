// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/FirstPersonBase.h"
#include "ItemData.h"
#include "WeaponItem.h"
#include "EquipSlotComponent.h"
#include "InventoryComponent.h"

// Sets default values
AFirstPersonBase::AFirstPersonBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthCmp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	EquipSlotsCmp = CreateDefaultSubobject<UEquipSlotComponent>(TEXT("EquipSlotComponent"));
	InventoryCmp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	if (HealthCmp)
	{
		HealthCmp->OnDeath.AddDynamic(this, &AFirstPersonBase::OnDeath);
		HealthCmp->OnHealthChanged.AddDynamic(this, &AFirstPersonBase::OnHealthChanged);
	}

	if (EquipSlotsCmp)
		EquipSlotsCmp->OnUnequipItem.AddDynamic(this, &AFirstPersonBase::OnUnequipItem);

	if (InventoryCmp)
		InventoryCmp->OnPickup.AddDynamic(this, &AFirstPersonBase::OnPickUpItem);


}

int32 AFirstPersonBase::GetAllOwningItemCount(const UItemData* item) const
{
	return InventoryCmp->GetItemCount(item);
}


FInventorySlot AFirstPersonBase::GetEquipSlot(EEquipType type) const
{
	return EquipSlotsCmp->GetEquipItemSlot(type);
}

bool AFirstPersonBase::HasWeapon()
{
	return GetEquipSlot(EEquipType::kWeapon).GetItem() != nullptr;
}

int AFirstPersonBase::GetCurrentAmmoCount()
{
	return GetEquipSlot(EEquipType::kWeapon).GetQuantity();
}

int AFirstPersonBase::GetTotalAmmoCount()
{
	int totalAmmo = 0;

	const TArray<FInventorySlot>& inventorySlots = InventoryCmp->GetSlots();

	if (inventorySlots.IsEmpty())
		return 0;

	for (const auto& slot : inventorySlots)
	{
		const UItemData* item = slot.GetItem();
		if (item->m_itemType == EItemType::kAmmo)
		{
			totalAmmo += slot.GetQuantity();
		}
	}

	return totalAmmo;
}

int AFirstPersonBase::GetTotalHealKitCount()
{
	int totalHealKit = 0;

	const TArray<FInventorySlot>& inventorySlots = InventoryCmp->GetSlots();

	if (inventorySlots.IsEmpty())
		return 0;

	for (const auto& slot : inventorySlots)
	{
		const UItemData* item = slot.GetItem();
		if (item->m_itemType == EItemType::kConsumable)
		{
			totalHealKit += slot.GetQuantity();
		}
	}

	return totalHealKit;
}

bool AFirstPersonBase::TryReload()
{
	const FInventorySlot& weaponSlot = EquipSlotsCmp->GetEquipItemSlot(EEquipType::kWeapon);
	UWeaponItem* currentWeapon = Cast<UWeaponItem>(weaponSlot.GetItem());

	if (!IsValid(currentWeapon))
		return false;

	int32 amountAmmoNeed = currentWeapon->GetMaxAmmoCount() - weaponSlot.GetQuantity();

	if (amountAmmoNeed <= 0)
		return false;

	const TObjectPtr<UItemData>& ammoItem = currentWeapon->GetUsingAmmo();

	int32 ownedAmmoCount = InventoryCmp->GetItemCount(ammoItem);
	if (ownedAmmoCount <= 0)
		return false;

	int32 reloadingCount = amountAmmoNeed;

	if (ownedAmmoCount < amountAmmoNeed)
		reloadingCount = ownedAmmoCount;

	InventoryCmp->RemoveItem(ammoItem, reloadingCount);

	EquipSlotsCmp->AddItemQuantity(currentWeapon->m_equipType, reloadingCount);

	return true;
}

void AFirstPersonBase::EquipItem(UItemData* item)
{
	EquipSlotsCmp->EquipItem(item);
}

bool AFirstPersonBase::TryFireWeapon(FVector direction)
{
	const UWeaponItem* currentWeapon = Cast<UWeaponItem>(EquipSlotsCmp->GetEquipItemSlot(EEquipType::kWeapon).GetItem());

	if (!IsValid(currentWeapon))
		return false;

	if (EquipSlotsCmp->GetEquipItemSlot(currentWeapon->m_equipType).GetQuantity() <= 0)
		return false;

	const UItemData* usingAmmo = currentWeapon->GetUsingAmmo();

	if (!IsValid(usingAmmo))
		return false;

	return true;
}

// Called when the game starts or when spawned
void AFirstPersonBase::BeginPlay()
{
	Super::BeginPlay();

}

void AFirstPersonBase::OnPickUpItem(UItemData* item)
{
	//// check if the item can be equip and slot is empty
	const EEquipType& equipType = item->m_equipType;

	if (EquipSlotsCmp->IsSlotEmpty(equipType) && equipType != EEquipType::kNone)
	{
		if (equipType == EEquipType::kWeapon)
		{
			EquipSlotsCmp->EquipItem(item);
			//m_pWeapon = Cast<UWeaponItem>(item);
		}
	}
}

void AFirstPersonBase::OnUnequipItem(UItemData* item)
{

}

void AFirstPersonBase::OnHealthChanged(UHealthComponent* pHealthCmp, float newHealth, float delta, AActor* damageCauser)
{
	if (delta < 0.0f && IsValid(damageCauser))
	{
		m_isInCombat = true;
	}

	if (newHealth <= 0)
		OnDeath();
}

void AFirstPersonBase::OnDeath()
{
}

// Called every frame
void AFirstPersonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void AFirstPersonBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
