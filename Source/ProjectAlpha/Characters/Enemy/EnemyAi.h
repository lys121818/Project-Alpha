// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../FirstPersonBase.h"
#include "HealthComponent.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "TargetData.h"
#include "EnemyAi.generated.h"
// struct class of current state

class UInventoryComponent;
class UItemData;
class UEquipSlotComponent;
class USphereComponent;
class UBehaviorTree;
class ATargetPoint;
class UAiDecision;
class APickupItem;
class ARegionArea;

UCLASS()
class PROJECTALPHA_API AEnemyAi : public AFirstPersonBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAi();


	UFUNCTION()
	bool TryPickupItem(APickupItem* item);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnDeath() override;

	virtual void OnHealthChanged(UHealthComponent* healthCmp, float newHealth, float delta, AActor* damageCauser) override;

	void OnUnequipped(UItemData* item);

	virtual void OnPickUpItem(UItemData* item) override;

	virtual bool TryFireWeapon(FVector direction) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool TryUseHealKitItem() override;

	void OnRegionChanged();

	TObjectPtr<ARegionArea> GetLocatedRegion();



	// fire towards actor
	UFUNCTION()
	bool TryFireAt(AActor* actor);

	const TArray<TObjectPtr<UAiDecision>>& GetDecisions();

protected:
	UPROPERTY(EditAnywhere, Category = "Collision")
	TObjectPtr<USphereComponent> m_pSphere;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> m_behaviorTree;

	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<TObjectPtr<UAiDecision>> Decisions;

	UPROPERTY(EditAnywhere, Category = "Region")
	TObjectPtr<ARegionArea> LocatedRegion;


private:
	int m_nextTargetIndex = 0;
	
};
