// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "InventorySlot.h"
#include "EquipSlotComponent.h"
#include "FirstPersonBase.generated.h"

class UEquipSlotComponent;
class UInventoryComponent;
class UWeaponItem;
class UItemData;
class ARandomPickupItem;

UCLASS()
class PROJECTALPHA_API AFirstPersonBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFirstPersonBase();

	UFUNCTION()
	float GetHealth() const { return HealthCmp->m_currentHealth; }
	UFUNCTION()
	float GetMaxHealth() const { return HealthCmp->m_maxHealth; }

	int32 GetAllOwningItemCount(const UItemData* item) const;

	FInventorySlot GetEquipSlot(EEquipType type) const;

	UInventoryComponent* GetInventoryComponent() { return InventoryCmp; }

	UFUNCTION()
	bool HasWeapon();

	UFUNCTION()
	int GetCurrentAmmoCount();
	int GetTotalAmmoCount();

	int GetTotalHealKitCount();

	bool IsInCombat() const { return m_isInCombat; }
	void SetCombat(bool inCombat) { m_isInCombat = inCombat; }


	virtual bool TryUseHealKitItem() PURE_VIRTUAL(AFirstPersonBase::UseHealKitItem, return false;);
	bool TryReload();
	void HealHealth(float amount) { HealthCmp->Heal(amount); }
	void EquipItem(UItemData* item);

	UFUNCTION()
	virtual bool TryFireWeapon(FVector direction = FVector::Zero());

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthCmp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UEquipSlotComponent* EquipSlotsCmp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInventoryComponent* InventoryCmp;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnPickUpItem(UItemData* item);

	UFUNCTION()
	virtual void OnUnequipItem(UItemData* item);

	UFUNCTION()
	virtual void OnHealthChanged(UHealthComponent* pHealthCmp, float newHealth, float delta, AActor* damageCauser);
	
	UFUNCTION()
	virtual void OnDeath();

	float m_fireCD = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	bool m_isInCombat = false;
};
