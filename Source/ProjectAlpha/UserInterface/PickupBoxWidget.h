// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupBoxWidget.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UPickupBoxWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// =============================================
	// Public Callable Functions
	// =============================================
	void Init(UInventoryComponent* playerInventory, UInventoryComponent* boxInventory);

	UFUNCTION(BlueprintCallable)
	void TransferItemToPlayer(int slotIndex);

	UFUNCTION(BlueprintCallable)
	void TransferItemToBox(int slotIndex);

private:
	// =============================================
	// Member Variables
	// =============================================
	UPROPERTY()
	TObjectPtr<UInventoryComponent> PlayerInventory;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> BoxInventory;
};
