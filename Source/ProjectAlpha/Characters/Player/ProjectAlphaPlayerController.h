// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InventoryComponent.h"
#include "EquipSlotComponent.h"
#include "ProjectAlphaPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;
class UInventoryWidget;
class UPickupBoxWidget;
class APickupItem;
class AItemLootBox;


/**
 *  Simple first person Player Controller
 *  Manages the input mapping context.
 *  Overrides the Player Camera Manager class.
 */
UCLASS(abstract)
class PROJECTALPHA_API AProjectAlphaPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	/** Constructor */
	AProjectAlphaPlayerController();


	// Inventory
	UFUNCTION(BlueprintCallable)
	void ToggleInventory(UInventoryComponent* pInventoryCmp);

	void EquipItem();

	void AddToInventory(APickupItem* item);

	// Pickup Box
	UFUNCTION(BlueprintCallable)
	void OpenPickupBox(UInventoryComponent* playerInventory, UInventoryComponent* boxInventory, AItemLootBox* lootBox);

	UFUNCTION(BlueprintCallable)
	void ClosePickupBox();

protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;


	UFUNCTION(BlueprintCallable)
	void OpenInventory(UInventoryComponent* pInventoryCmp);

	UFUNCTION(BlueprintCallable)
	void CloseInventory();

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	bool m_isInventoryOpen = false;

	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget = nullptr;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UPickupBoxWidget> PickupBoxWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	bool bIsPickupBoxOpen = false;

	UPROPERTY()
	TObjectPtr<UPickupBoxWidget> PickupBoxWidget = nullptr;

	/** Loot box currently shown in PickupBoxWidget; discovery is paused on this when ClosePickupBox() is called. */
	UPROPERTY()
	TObjectPtr<AItemLootBox> CurrentLootBox = nullptr;

};
