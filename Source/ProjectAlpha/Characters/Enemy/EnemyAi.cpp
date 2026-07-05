// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAi.h"
#include "InventoryComponent.h"
#include "EquipSlotComponent.h"
#include "ItemData.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PickupItem.h"
#include "ProjectAlphaCharacter.h"
#include "EnemyAIController.h"
#include "AIController.h"
#include "WeaponItem.h"
#include "RegionAreaSubsystem.h"

// Sets default values
AEnemyAi::AEnemyAi()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Sphere component for pickup items
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	m_pSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	m_pSphere->SetupAttachment(GetCapsuleComponent());

	m_pSphere->InitSphereRadius(120.f);
	m_pSphere->SetCollisionObjectType(ECC_Pawn);
	m_pSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_pSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_pSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	m_pSphere->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void AEnemyAi::BeginPlay()
{
	Super::BeginPlay();

	// get behaviortree from controller 
	AAIController* controller = Cast<AAIController>(GetController());
	if (controller && m_behaviorTree)
		controller->RunBehaviorTree(m_behaviorTree);


	OnRegionChanged();
}

void AEnemyAi::OnDeath()
{
	Destroy();
}

void AEnemyAi::OnHealthChanged(UHealthComponent* healthCmp, float newHealth, float delta, AActor* damageCauser)
{
	Super::OnHealthChanged(healthCmp, newHealth, delta, damageCauser);

	AEnemyAIController* controller = Cast<AEnemyAIController>(Controller);

	if(IsValid(controller))
		controller->AddCausedTarget(damageCauser);

	if (delta < 0.0f)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 1.0f, FColor::Red,
				FString::Printf(TEXT("Enemy Hp: %.0f"), newHealth)
				);
		}
	}

	if (newHealth <= 0)
		OnDeath();
}

void AEnemyAi::OnUnequipped(UItemData* item)
{
	Super::OnUnequipItem(item);
	// all equipments are amount of single object
	InventoryCmp->AddItem(item, item->m_count);
}

bool AEnemyAi::TryPickupItem(APickupItem* item)
{
	if (!IsValid(item))
		return false;

	return item->TryPickup(this);
}

void AEnemyAi::OnPickUpItem(UItemData* item)
{
	Super::OnPickUpItem(item);

}


bool AEnemyAi::TryFireWeapon(FVector direction)
{
	bool success = Super::TryFireWeapon(direction);

	if (!success)
		return false;

	// can't fire
	if (!HasWeapon() || m_fireCD > 0)
		return false;

	// get player location
	AEnemyAIController* controller = Cast<AEnemyAIController>(GetController());

	if (!controller)
		return false;

	FVector viewLocation;
	FRotator viewRotation;

	controller->GetPlayerViewPoint(viewLocation, viewRotation);

	// set raytracing points
	const FVector startLocation = viewLocation;
	const FVector endLocation = startLocation + (direction * 10000.0f);

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
	const FVector debugEndPoint = isHit ? hit.ImpactPoint : endLocation;
	const FColor debugLineColor = isHit ? FColor::Red : FColor::Blue;

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
#endif

	if (isHit)
	{
		// Do damage
		UGameplayStatics::ApplyPointDamage(
			hit.GetActor(),
			10.0f,
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
	m_isInCombat = true;
	return true;
}

// Called every frame
void AEnemyAi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_fireCD -= DeltaTime;
}

// Called to bind functionality to input
void AEnemyAi::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AEnemyAi::TryUseHealKitItem()
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

void AEnemyAi::OnRegionChanged()
{
	URegionAreaSubsystem* regionSubsystem = GetWorld()->GetSubsystem<URegionAreaSubsystem>();

	ARegionArea* currentRegion = regionSubsystem->FindRegionForActor(this);

	if (!IsValid(currentRegion))
		return;

	LocatedRegion = currentRegion;

	AEnemyAIController* pController = Cast<AEnemyAIController>(GetController());

	if (!IsValid(pController))
		return;

	pController->SetRegionVisited(LocatedRegion);
}

TObjectPtr<ARegionArea> AEnemyAi::GetLocatedRegion()
{
	URegionAreaSubsystem* regionSubsystem = GetWorld()->GetSubsystem<URegionAreaSubsystem>();

	ARegionArea* currentRegion = regionSubsystem->FindRegionForActor(this);

	if (!IsValid(currentRegion))
		return nullptr;

	if (LocatedRegion != currentRegion)
	{
		LocatedRegion = currentRegion;
		AEnemyAIController* pController = Cast<AEnemyAIController>(GetController());

		if (!IsValid(pController))
			return nullptr;

		pController->SetRegionVisited(LocatedRegion);
	}

	return LocatedRegion;
}

bool AEnemyAi::TryFireAt(AActor* actor)
{
	if (!IsValid(actor) || !HasWeapon())
		return false;

	if (m_fireCD > 0)
		return false;

	// get direction between this and target
	FVector myLocation = GetActorLocation();
	FVector targetLocation = actor->GetActorLocation();

	FVector direction = (targetLocation - myLocation).GetSafeNormal();

	// set rotation with fixed y rotation
	FRotator lookAt = FVector(direction.X,direction.Y,0.0f).Rotation();
	SetActorRotation(lookAt);

	// fire
	return TryFireWeapon(direction);
}

const TArray<TObjectPtr<UAiDecision>>& AEnemyAi::GetDecisions()
{
	return Decisions;
}

