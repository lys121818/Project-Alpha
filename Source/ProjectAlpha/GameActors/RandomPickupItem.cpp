// Fill out your copyright notice in the Description page of Project Settings.


#include "GameActors/RandomPickupItem.h"
#include "Logging/LogMacros.h"
#include "ItemData.h"
#include "Math/RandomStream.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

ARandomPickupItem::ARandomPickupItem()
{
}

void ARandomPickupItem::BeginPlay()
{
    Super::BeginPlay();

    GenerateRandomItem();

    if (IsValid(m_itemData))
        m_amount = m_itemData->m_count;
}

void ARandomPickupItem::GenerateRandomItem()
{
    if (m_itemDataList.IsEmpty())
        return;

    int max = m_itemDataList.Num();
    FRandomStream rng;
    rng.GenerateNewSeed();


    int random = rng.RandRange(0, max - 1);
   

    m_itemData = m_itemDataList[random];
    
    // set static mesh 
    m_mesh->SetStaticMesh(m_itemData->m_staticMesh);
    m_mesh->SetRelativeScale3D(m_itemData->m_worldScale);

}
