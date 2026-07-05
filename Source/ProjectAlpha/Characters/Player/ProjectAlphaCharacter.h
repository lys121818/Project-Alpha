// Copyright Epic Games, Inc. All Rights Reserved.
#define DEBUG_MODE 1

#pragma once

#include "CoreMinimal.h"
#include "../FirstPersonBase.h"
#include "Logging/LogMacros.h"
#include "HealthComponent.h"

#include "ProjectAlphaCharacter.generated.h"

class UInputComponent;
class USphereComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class APickupItem;
class AItemLootBox;

class ATargetPoint;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);



/**
 *  A basic first person character
 */
UCLASS(abstract)
class AProjectAlphaCharacter : public AFirstPersonBase
{
	GENERATED_BODY()

public:
	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;




protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* FiringAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* InteractionAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* HealAction;

public:
	AProjectAlphaCharacter();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void FireWeapon();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnInteraction();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void TryOpenInventory();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnReload();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnHeal();


	virtual bool TryFireWeapon(FVector direction = FVector::Zero()) override;

	virtual void OnPickUpItem(UItemData* item) override;
	virtual void OnUnequipItem(UItemData* item) override;

	virtual void OnDeath() override;

	virtual void OnHealthChanged(UHealthComponent* healthCmp, float newHealth, float delta, AActor* damageCauser) override;


private:
	void TryPickupItem(APickupItem* item);
	void TryOpenLootBox(AItemLootBox* lootBox);

protected:

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	
public:

	/** Returns the first person mesh **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	virtual bool TryUseHealKitItem() override;

	// Variables
protected:
	UPROPERTY(EditAnywhere, Category="Combat")
	float m_fireRange;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float m_damage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool m_drawDebugTrace;

	UPROPERTY(EditAnywhere, Category = "Collision")
	TObjectPtr<USphereComponent> m_sphere;



#ifdef DEBUG_MODE
	void DebugPrintOnScreen(FString string, FColor color = FColor::Red);

	void TestFire();
	bool CheckRaycastHit(FHitResult& result, FVector start, FVector end, AActor* ignore = nullptr);
#endif
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test")
	TObjectPtr<ATargetPoint> Testtarget;

};

