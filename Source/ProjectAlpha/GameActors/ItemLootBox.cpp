#include "ItemLootBox.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "InventoryComponent.h"
#include "ItemData.h"

// =============================================
// On Build Init
// =============================================

AItemLootBox::AItemLootBox()
{
    PrimaryActorTick.bCanEverTick = false;

    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    SetRootComponent(InteractionSphere);
    InteractionSphere->InitSphereRadius(80.f);
    InteractionSphere->SetCollisionObjectType(ECC_WorldDynamic);
    InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    InteractionSphere->SetGenerateOverlapEvents(true);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(InteractionSphere);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    BoxInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("BoxInventory"));
}

// =============================================
// On Run Init
// =============================================

void AItemLootBox::BeginPlay()
{
    Super::BeginPlay();

    GenerateLootPool();
}

// =============================================
// On Exit
// =============================================

void AItemLootBox::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    StopDiscovering();

    Super::EndPlay(endPlayReason);
}

// =============================================
// Public Callable Functions
// =============================================

void AItemLootBox::Open()
{
    StartDiscovering();
}

void AItemLootBox::StopDiscovering()
{
    GetWorldTimerManager().ClearTimer(DiscoverTimerHandle);
}

// =============================================
// Private Core Functions
// =============================================

void AItemLootBox::GenerateLootPool()
{
    if (ItemPool.IsEmpty())
        return;

    for (int32 i = 0; i < LootCount; ++i)
    {
        const int32 randomIndex = FMath::RandRange(0, ItemPool.Num() - 1);
        UItemData* item = ItemPool[randomIndex];

        if (!IsValid(item))
            continue;

        FInventorySlot slot;
        slot.SetItemData(item, item->m_count);
        HiddenLoot.Add(slot);
    }
}

void AItemLootBox::DiscoverNextItem()
{
    if (HiddenLoot.IsEmpty())
    {
        StopDiscovering();
        return;
    }

    const FInventorySlot slot = HiddenLoot[0];
    HiddenLoot.RemoveAt(0);

    if (UItemData* item = slot.GetItem())
        BoxInventory->AddItem(item, slot.GetQuantity());

    if (HiddenLoot.IsEmpty())
        StopDiscovering();
}

// =============================================
// Private Sub Core Helper Functions
// =============================================

void AItemLootBox::StartDiscovering()
{
    if (HiddenLoot.IsEmpty())
        return;

    if (GetWorldTimerManager().IsTimerActive(DiscoverTimerHandle))
        return;

    GetWorldTimerManager().SetTimer(DiscoverTimerHandle, this, &AItemLootBox::DiscoverNextItem, DiscoverInterval, true);
}
