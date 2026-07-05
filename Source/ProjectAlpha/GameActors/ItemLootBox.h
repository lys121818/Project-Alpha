#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventorySlot.h"
#include "ItemLootBox.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UInventoryComponent;
class UItemData;

/**
 * LootBox filled with random items from the list when spawn. Each Item in lootbox can be discovered each second. Player can be interactable with drag and drop to player's inventory.
 */
UCLASS()
class PROJECTALPHA_API AItemLootBox : public AActor
{
    GENERATED_BODY()

public:
    // =============================================
    // On Build Init
    // =============================================
    /** Creates the interaction sphere (root), mesh, and box inventory components. */
    AItemLootBox();

protected:
    // =============================================
    // On Run Init
    // =============================================
    virtual void BeginPlay() override;

    // =============================================
    // On Exit
    // =============================================
    virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

public:
    // =============================================
    // Public Callable Functions
    // =============================================
    /** (Re)starts revealing items from HiddenLoot if discovery isn't already running. Safe to call again to resume after StopDiscovering(). */
    UFUNCTION(BlueprintCallable)
    void Open();

    /** Pauses item discovery by clearing DiscoverTimerHandle. Call when the player stops interacting; Open() resumes from the remaining HiddenLoot entries. */
    UFUNCTION(BlueprintCallable)
    void StopDiscovering();

    /** Returns the inventory holding items that have already been revealed from HiddenLoot. */
    UInventoryComponent* GetBoxInventory() const { return BoxInventory; }

private:
    // =============================================
    // Private Core Functions
    // =============================================
    /** Rolls LootCount random entries from ItemPool into HiddenLoot. Called once from BeginPlay. */
    void GenerateLootPool();

    /** Pops the front entry of HiddenLoot into BoxInventory and stops discovery once HiddenLoot is empty. */
    void DiscoverNextItem();

    // =============================================
    // Private Sub Core Helper Functions
    // =============================================
    /** Starts DiscoverTimerHandle if HiddenLoot has entries left and discovery isn't already running. */
    void StartDiscovering();

    // =============================================
    // Member Variables
    // =============================================
    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<USphereComponent> InteractionSphere;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> Mesh;

    /** Holds items already revealed from HiddenLoot; exposed via GetBoxInventory() to UPickupBoxWidget. */
    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UInventoryComponent> BoxInventory;

    /** Candidate item pool that GenerateLootPool() rolls from to fill HiddenLoot. */
    UPROPERTY(EditAnywhere, Category = "Loot")
    TArray<TObjectPtr<UItemData>> ItemPool;

    UPROPERTY(EditAnywhere, Category = "Loot", meta = (ClampMin = "1"))
    int32 LootCount = 3;

    UPROPERTY(EditAnywhere, Category = "Loot", meta = (ClampMin = "0.1"))
    float DiscoverInterval = 1.0f;

    /** Loot rolled on spawn but not yet revealed; drained into BoxInventory by DiscoverNextItem(). */
    TArray<FInventorySlot> HiddenLoot;

    FTimerHandle DiscoverTimerHandle;
};
