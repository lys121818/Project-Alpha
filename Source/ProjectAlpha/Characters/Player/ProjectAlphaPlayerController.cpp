// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectAlphaPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "ProjectAlphaCameraManager.h"
#include "Blueprint/UserWidget.h"
#include "ProjectAlpha.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "InventoryWidget.h"
#include "PickupBoxWidget.h"
#include "ItemData.h"
#include "FirstPersonBase.h"
#include "ItemLootBox.h"

AProjectAlphaPlayerController::AProjectAlphaPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AProjectAlphaCameraManager::StaticClass();




}

void AProjectAlphaPlayerController::ToggleInventory(UInventoryComponent* pInventoryCmp)
{
	if (m_isInventoryOpen)
	{
		CloseInventory();
	}
	else
	{
		OpenInventory(pInventoryCmp);
	}
}

void AProjectAlphaPlayerController::EquipItem()
{
}

void AProjectAlphaPlayerController::AddToInventory(APickupItem* item)
{
}



void AProjectAlphaPlayerController::OpenInventory(UInventoryComponent* pInventoryCmp)
{
	// 
	if (!IsValid(pInventoryCmp))
		return;

	const TArray<FInventorySlot>& slots = pInventoryCmp->GetSlots();

	if (!IsValid(InventoryWidget))
	{
		return;
	}

	InventoryWidget->Init(pInventoryCmp);

	// set mouse visiable
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	SetInputMode(InputMode);
	bShowMouseCursor = true;


	m_isInventoryOpen = true;
	InventoryWidget->UpdateInventory();
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);

}

void AProjectAlphaPlayerController::CloseInventory()
{
	if (!InventoryWidget)
	{
		return;
	}

	// set mouse invisiable
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;

	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	m_isInventoryOpen = false;
}

void AProjectAlphaPlayerController::OpenPickupBox(UInventoryComponent* playerInventory, UInventoryComponent* boxInventory, AItemLootBox* lootBox)
{
	if (!IsValid(playerInventory) || !IsValid(boxInventory))
		return;

	if (!IsValid(PickupBoxWidget))
		return;

	PickupBoxWidget->Init(playerInventory, boxInventory);
	CurrentLootBox = lootBox;

	// set mouse visiable
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(PickupBoxWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	SetInputMode(InputMode);
	bShowMouseCursor = true;

	bIsPickupBoxOpen = true;
	PickupBoxWidget->SetVisibility(ESlateVisibility::Visible);
}

void AProjectAlphaPlayerController::ClosePickupBox()
{
	if (!PickupBoxWidget)
	{
		return;
	}

	// set mouse invisiable
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;

	if (IsValid(CurrentLootBox))
		CurrentLootBox->StopDiscovering();
	CurrentLootBox = nullptr;

	PickupBoxWidget->SetVisibility(ESlateVisibility::Hidden);
	bIsPickupBoxOpen = false;
}

void AProjectAlphaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AFirstPersonBase* character = Cast<AFirstPersonBase>(GetPawn());


	// create Inventory
	if (IsLocalController() && InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
		if (InventoryWidget)
		{
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	// create Pickup Box
	if (IsLocalController() && PickupBoxWidgetClass)
	{
		PickupBoxWidget = CreateWidget<UPickupBoxWidget>(GetWorld(), PickupBoxWidgetClass);
		if (PickupBoxWidget)
		{
			PickupBoxWidget->AddToViewport();
			PickupBoxWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogProjectAlpha, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AProjectAlphaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}
	
