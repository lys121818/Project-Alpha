// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	m_maxHealth = 100.0f;
	m_currentHealth = m_maxHealth;

	// ...
	if (AActor* owner = GetOwner())
	{
		owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeDamage);
	}

	m_currentHealth = m_maxHealth;
}

void UHealthComponent::HandleTakeDamage(AActor* damagedActor, float damageAmount, const class UDamageType* damageType, class AController* instigateBy, AActor* damageCauser)
{
	if (damageAmount <= 0.0f || IsDead())
		return;

	const float oldHealth = m_currentHealth;
	m_currentHealth = FMath::Clamp(m_currentHealth - damageAmount, 0.0f, m_maxHealth);

	OnHealthChanged.Broadcast(this, m_currentHealth, m_currentHealth - oldHealth, damageCauser);

	// dead after damage
	if (IsDead())
		OnDeath.Broadcast();
}


void UHealthComponent::Heal(float amount)
{
	const float newHealth = m_currentHealth + amount;
	const float oldHealth = m_currentHealth;
	
	// clamp health from over healing
	m_currentHealth = (newHealth > m_maxHealth) ? m_maxHealth : newHealth;

	OnHealthChanged.Broadcast(this, newHealth, m_currentHealth - oldHealth, nullptr);
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

