// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameActors/PickupItem.h"
#include "RandomPickupItem.generated.h"

/**
 *	
 */
UCLASS()
class PROJECTALPHA_API ARandomPickupItem : public APickupItem
{
	GENERATED_BODY()

public:
	ARandomPickupItem();

	virtual void BeginPlay() override;

	const TArray<TObjectPtr<UItemData>>& GetAllItemList() { return m_itemDataList; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item List")
	TArray<TObjectPtr<UItemData>> m_itemDataList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UStaticMeshComponent* StaticMesh;
private:
	void GenerateRandomItem();

};
