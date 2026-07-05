// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, UHealthComponent*, healthCmp, float, newHealth, float, delta, AActor*, damageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTALPHA_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();


	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeath OnDeath;

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead() const { return m_currentHealth <= 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float amount);

	// variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float m_maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float m_currentHealth;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTakeDamage(AActor* damagedActor, float damageAmount, const class UDamageType* damageType, class AController* instigateBy, AActor* damageCauser);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Variables
protected:

};
