// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySlot.h"
#include "EquipSlotComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnequipItem, UItemData*, Item);

class UItemData;

UENUM(BlueprintType)
enum class EEquipType : uint8
{
	kHead,
	kBody,
	kWeapon,
	kNone,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTALPHA_API UEquipSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipSlotComponent();

	UFUNCTION(BlueprintCallable)
	bool EquipItem(UItemData* item);

	UFUNCTION(BlueprintCallable)
	bool UnEquipItem(UItemData* type);

	FOnEquipItem OnEquipItem;
	FOnUnequipItem OnUnequipItem;

	TObjectPtr<UItemData> GetEquippedItem(EEquipType type) const { return m_equipSlots[type].GetItem(); }

	void AddItemQuantity(EEquipType type, int32 amount) { m_equipSlots[type].AddQuantity(amount); }
	const FInventorySlot& GetEquipItemSlot(EEquipType type) { return m_equipSlots[type]; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	void InitEquipSlots();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EEquipType, FInventorySlot> m_equipSlots;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// check for empty slot
	bool IsSlotEmpty(const EEquipType& type) const;
		
};
