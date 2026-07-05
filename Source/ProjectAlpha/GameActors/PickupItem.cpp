// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "InventoryComponent.h"
#include "ItemData.h"
#include "GameFramework/Character.h"
#include "InteractComponent.h"
#include "Logging/LogMacros.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "ProjectAlphaPlayerController.h"
#include "FirstPersonBase.h"

// Sets default values
APickupItem::APickupItem()
{
    PrimaryActorTick.bCanEverTick = false;

    // create components
    m_interactCmp = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
    m_sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    m_stimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

    SetRootComponent(m_sphere);
    m_sphere->InitSphereRadius(80.f);
    m_sphere->SetCollisionObjectType(ECC_WorldDynamic);
    m_sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    m_sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    m_sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    m_sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    m_sphere->SetGenerateOverlapEvents(true);

    m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    m_mesh->SetupAttachment(m_sphere);
    m_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    m_stimuliSource->bAutoRegister = false;
    
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();

    // registart as stimuli source
    if (m_stimuliSource)
    {
        m_stimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
        m_stimuliSource->RegisterWithPerceptionSystem();
    }

    if(IsValid(m_itemData))
        m_amount = m_itemData->m_count;

}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool APickupItem::TryPickup(AFirstPersonBase* character)
{
    if (!character || !m_itemData) return false;

    if (UInventoryComponent* Inv = character->FindComponentByClass<UInventoryComponent>())
    {
        const bool bAllAdded = Inv->AddItem(m_itemData, m_amount);

        if (bAllAdded)
        {
            Destroy();
            return true;
        }
    }

    return false;
}

const UItemData* APickupItem::GetItemData() const
{
    return m_itemData;
}

