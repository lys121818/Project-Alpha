// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

class UImage;
class UTextBlock;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class PROJECTALPHA_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(AActor* playerActor, UInventoryComponent* inventoryComp);

	void UpdateUserWidget();

protected:
	TObjectPtr<UTextBlock> AmmoTextBox;
	TObjectPtr<UTextBlock> HealthTextBox;
	TObjectPtr<UImage> CrossHair;

	TWeakObjectPtr<UInventoryComponent> ownerInventoryComp;
};
