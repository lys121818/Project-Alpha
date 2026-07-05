// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectAlphaCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectAlpha.h"
#include "Components/SphereComponent.h"
#include "PickupItem.h"
#include "ItemLootBox.h"
#include "ItemData.h"
#include "ProjectAlphaPlayerController.h"
#include "Engine/TargetPoint.h"
#include "WorldMap.h"
#include "ProjectAlphaGameMode.h"
#include "WeaponItem.h"


AProjectAlphaCharacter::AProjectAlphaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// sphere collision
	m_sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	m_sphere->SetupAttachment(GetCapsuleComponent());

	m_sphere->InitSphereRadius(120.f);
	m_sphere->SetCollisionObjectType(ECC_Pawn);
	m_sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	m_sphere->SetGenerateOverlapEvents(true);

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	// variable settings
	m_fireRange = 10000.0f;
	m_damage = 10.0f;

	// sign components




#ifdef DEBUG_MODE
	m_drawDebugTrace = true;
#else
	m_drawDebugTrace = false;
#endif

}

void AProjectAlphaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AProjectAlphaCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AProjectAlphaCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectAlphaCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjectAlphaCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AProjectAlphaCharacter::LookInput);

		EnhancedInputComponent->BindAction(FiringAction, ETriggerEvent::Started, this, &AProjectAlphaCharacter::FireWeapon);
		EnhancedInputComponent->BindAction(FiringAction, ETriggerEvent::Ongoing, this, &AProjectAlphaCharacter::FireWeapon);

		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &AProjectAlphaCharacter::OnInteraction);

		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &AProjectAlphaCharacter::TryOpenInventory);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AProjectAlphaCharacter::OnReload);

		EnhancedInputComponent->BindAction(HealAction, ETriggerEvent::Triggered, this, &AProjectAlphaCharacter::OnHeal);
	}
	else
	{
		UE_LOG(LogProjectAlpha, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

bool AProjectAlphaCharacter::TryUseHealKitItem()
{
	const TArray<FInventorySlot>& inventorySlots = InventoryCmp->GetSlots();

	if (inventorySlots.IsEmpty())
		return false;

	for (const auto& slot : inventorySlots)
	{
		UItemData* item = slot.GetItem();
		if (item->m_itemType == EItemType::kConsumable)
		{
			item->UseItem(this);
			return true;
		}
	}

	return false;
}


void AProjectAlphaCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectAlphaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectAlphaCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void AProjectAlphaCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void AProjectAlphaCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AProjectAlphaCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AProjectAlphaCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AProjectAlphaCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void AProjectAlphaCharacter::FireWeapon()
{
	TryFireWeapon();
}

void AProjectAlphaCharacter::OnInteraction()
{
	AProjectAlphaPlayerController* controller = Cast<AProjectAlphaPlayerController>(Controller);

	// check for overlapping objects / actors to interact
	// items
	TArray<AActor*> itemOverlaps;



	// priority matters here
	m_sphere->GetOverlappingActors(itemOverlaps, APickupItem::StaticClass());

	 
	if (itemOverlaps.Num() > 0)
	{
		APickupItem* item = nullptr;

		float bestDistSq = FLT_MAX;
		const FVector myLocation = GetActorLocation();

		for (AActor* act : itemOverlaps)
		{
			if (!act) continue;

			const float distSq = FVector::DistSquared(myLocation, act->GetActorLocation());
			if (distSq < bestDistSq)
			{
				bestDistSq = distSq;
				item = Cast<APickupItem>(act);
			}
		}

		TryPickupItem(item);
		return;
	}

	// loot boxes
	TArray<AActor*> lootBoxOverlaps;
	m_sphere->GetOverlappingActors(lootBoxOverlaps, AItemLootBox::StaticClass());

	if (lootBoxOverlaps.Num() > 0)
	{
		AItemLootBox* lootBox = nullptr;

		float bestDistSq = FLT_MAX;
		const FVector myLocation = GetActorLocation();

		for (AActor* act : lootBoxOverlaps)
		{
			if (!act) continue;

			const float distSq = FVector::DistSquared(myLocation, act->GetActorLocation());
			if (distSq < bestDistSq)
			{
				bestDistSq = distSq;
				lootBox = Cast<AItemLootBox>(act);
			}
		}

		TryOpenLootBox(lootBox);
		return;
	}

}


void AProjectAlphaCharacter::TryOpenInventory()
{
	AProjectAlphaPlayerController* controller = Cast<AProjectAlphaPlayerController>(Controller);

	controller->ToggleInventory(InventoryCmp);
}

void AProjectAlphaCharacter::OnReload()
{
	Super::TryReload();
}

void AProjectAlphaCharacter::OnHeal()
{
	if (GetTotalHealKitCount() > 0)
		TryUseHealKitItem();
}

bool AProjectAlphaCharacter::TryFireWeapon(FVector direction)
{
	bool success = Super::TryFireWeapon(direction);

	if (!success)
		return false;

	AProjectAlphaPlayerController* controller = Cast<AProjectAlphaPlayerController>(Controller);

	if (!HasWeapon())
		return false;
	// get player location

	if (!controller)
		return false;

	FVector viewLocation;
	FRotator viewRotation;

	controller->GetPlayerViewPoint(viewLocation, viewRotation);

	// set raytracing points
	const FVector startLocation = viewLocation;
	const FVector endLocation = startLocation + (viewRotation.Vector() * m_fireRange);

	// collision check of ray cast
	FHitResult hit;
	FCollisionQueryParams params(SCENE_QUERY_STAT(FireTrace), true);
	params.AddIgnoredActor(this);

	if (APawn* myPawn = controller->GetPawn())
		params.AddIgnoredActor(myPawn);

	const bool isHit = GetWorld()->LineTraceSingleByChannel(
		hit,
		startLocation,
		endLocation,
		ECC_Visibility,
		params
	);
#ifdef DEBUG_MODE
	// draw raycast line
	if (m_drawDebugTrace)
	{
		const FVector debugEndPoint = isHit ? hit.ImpactPoint : endLocation;
		const FColor debugLineColor = isHit ? FColor::Red : FColor::Green;

		DrawDebugLine(
			GetWorld(),
			startLocation,
			debugEndPoint,
			debugLineColor,
			false,
			1.0f,
			0,
			1.0f
		);
	}
#endif

	if (isHit)
	{
		// Do damage
		UGameplayStatics::ApplyPointDamage(
			hit.GetActor(),
			m_damage,
			viewRotation.Vector(),
			hit,
			controller,
			this,
			UDamageType::StaticClass()
		);
	}

	// add cd & decrease ammo
	const FInventorySlot& weaponSlot = EquipSlotsCmp->GetEquipItemSlot(EEquipType::kWeapon);
	UWeaponItem* pWeapon = Cast<UWeaponItem>(weaponSlot.GetItem());
	m_fireCD = pWeapon->GetDurationBetweenFire();
	EquipSlotsCmp->AddItemQuantity(pWeapon->m_equipType, -1);

#ifdef DEBUG_MODE
	const FInventorySlot& slot = EquipSlotsCmp->GetEquipItemSlot(EEquipType::kWeapon);
	UWeaponItem* weapon = Cast<UWeaponItem>(slot.GetItem());
	const int32 totalAmmo = InventoryCmp->GetItemCount(weapon->GetUsingAmmo());


	FString debugPrint = FString::Printf(TEXT("Ammo %d / %d"), slot.GetQuantity(), totalAmmo);
	DebugPrintOnScreen(debugPrint);
#endif
	return true;
}

void AProjectAlphaCharacter::OnPickUpItem(UItemData* item)
{
	Super::OnPickUpItem(item);
}

void AProjectAlphaCharacter::OnDeath()
{
	Super::OnDeath();

	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AProjectAlphaCharacter::OnHealthChanged(UHealthComponent* healthCmp, float newHealth, float delta, AActor* damageCauser)
{
	Super::OnHealthChanged(healthCmp, newHealth, delta, damageCauser);

	if (delta < 0.0f)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 1.0f, FColor::Red,
				FString::Printf(TEXT("Player Hp: %.0f"), newHealth)
			);
		}
	}

	if (newHealth <= 0)
		OnDeath();
}

void AProjectAlphaCharacter::TryPickupItem(APickupItem* item)
{
	if (IsValid(item))
	{
		item->TryPickup(this);
	}
}

void AProjectAlphaCharacter::TryOpenLootBox(AItemLootBox* lootBox)
{
	if (!IsValid(lootBox))
		return;

	lootBox->Open();

	if (AProjectAlphaPlayerController* controller = Cast<AProjectAlphaPlayerController>(Controller))
		controller->OpenPickupBox(InventoryCmp, lootBox->GetBoxInventory(), lootBox);
}


void AProjectAlphaCharacter::OnUnequipItem(UItemData* m_item)
{
}


#ifdef DEBUG_MODE

void AProjectAlphaCharacter::DebugPrintOnScreen(FString string, FColor color)
{
	if (GEngine)
	{

		GEngine->AddOnScreenDebugMessage(
			-1, 1.0f, color,string
		);
	}
}

void AProjectAlphaCharacter::TestFire()
{
	AProjectAlphaPlayerController* controller = Cast<AProjectAlphaPlayerController>(Controller);

	if (!HasWeapon())
		return;
	// get player location

	if (!controller)
		return;

	if (Testtarget == nullptr)
		return;

	FVector viewLocation;
	FRotator viewRotation;

	controller->GetPlayerViewPoint(viewLocation, viewRotation);

	// set raytracing points
	const FVector startLocation = viewLocation;
	FVector targetLocation = Testtarget->GetActorLocation();

	AProjectAlphaGameMode* gameMode = Cast<AProjectAlphaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	AWorldMap* worldMap = gameMode->GetWorldMap();

	gameMode->CheckIfTargetVisible(startLocation, targetLocation, this);


}

bool AProjectAlphaCharacter::CheckRaycastHit(FHitResult& result, FVector start, FVector end, AActor* ignore)
{

	// collision check of ray cast
	FCollisionQueryParams params(SCENE_QUERY_STAT(FireTrace), true);
	params.AddIgnoredActor(this);

	if (ignore)
		params.AddIgnoredActor(ignore);

	bool isHit = GetWorld()->LineTraceSingleByChannel(
		result,
		start,
		end,
		ECC_Visibility,
		params
	);


	// draw raycast line
	if (m_drawDebugTrace)
	{
		const FVector debugEndPoint = isHit ? result.ImpactPoint : end;
		const FColor debugLineColor = isHit ? FColor::Red : FColor::Green;

		DrawDebugLine(
			GetWorld(),
			start,
			debugEndPoint,
			debugLineColor,
			false,
			20.0f,
			0,
			0.5f
		);
	}

	if (!isHit)
	{
		isHit = GetWorld()->SweepSingleByChannel(result, start, end, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(20.0f), params);
		const FColor debugLineColor = isHit ? FColor::Red : FColor::Green;

		DrawDebugSphere(GetWorld(), end, 20.0f, 16, debugLineColor, false, 20.0f, 0, .5f);
	}

	return isHit;
}

#endif