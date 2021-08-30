// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaCharacterHealthComponent.h"
#include "ArenaGameMode.h"

// Sets default values for this component's properties
UArenaCharacterHealthComponent::UArenaCharacterHealthComponent()
{
	HealthMax = 100;
	Health = HealthMax;
}

bool UArenaCharacterHealthComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}


bool UArenaCharacterHealthComponent::IsAlive() const
{
	return Health > 0.0f;
}


bool UArenaCharacterHealthComponent::IsFullHealth() const
{
	return Health == HealthMax;
}


float UArenaCharacterHealthComponent::GetHealth() const
{
	return Health;
}

float UArenaCharacterHealthComponent::GetHealthMax() const
{
	return HealthMax;
}


bool UArenaCharacterHealthComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;


	Health = NewHealth;

	if (ActualDelta != 0.0f)
	{
		MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
	}

	return ActualDelta != 0;
}

UArenaCharacterHealthComponent* UArenaCharacterHealthComponent::GetHealthComponent(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UArenaCharacterHealthComponent>(FromActor->GetComponentByClass(UArenaCharacterHealthComponent::StaticClass()));
	}

	return nullptr;
}

void UArenaCharacterHealthComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}
