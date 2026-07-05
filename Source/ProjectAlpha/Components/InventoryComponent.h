// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventorySlot.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, UItemData*, item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickup, UItemData*, item);

class UItemData;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTALPHA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:



	UPROPERTY(BlueprintAssignable)
	FOnInventoryChanged OnInventoryChanged;

	UPROPERTY(BlueprintAssignable)
	FOnPickup OnPickup;

	UFUNCTION(BlueprintCallable)
	bool AddItem(UItemData* item, int32 amount);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(UItemData* item, int32 amount);

	UFUNCTION(BlueprintCallable)
	int32 GetItemCount(const UItemData* item) const;

	UFUNCTION(BlueprintCallable)
	const TArray<FInventorySlot>& GetSlots() const { return Slots; }

	UFUNCTION(BlueprintCallable)
	int32 GetCapacity() const { return Capacity; }

	UFUNCTION(BlueprintCallable)
	int32 GetFilledSlotCount() const { return Slots.Num(); }


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 Capacity = 20;
protected:
	// Called when the game starts or when spawned

	UPROPERTY(VisibleAnywhere)
	TArray<FInventorySlot> Slots;

public:	
	// Called every frame

};
