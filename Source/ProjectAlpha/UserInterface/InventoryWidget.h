// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;
class UWrapBox;
class UInventorySlotWidget;

/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Init(UInventoryComponent* inventoryCmp);

	UFUNCTION(BlueprintCallable)
	void UpdateInventory();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> SlotBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent = nullptr;
	
};
