// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaGameplayFunctionLibrary.h"
#include "ArenaCharacterHealthComponent.h"

bool UArenaGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	if (!TargetActor) return false;

	UArenaCharacterHealthComponent* HealthComp = Cast<UArenaCharacterHealthComponent>(TargetActor->GetComponentByClass(UArenaCharacterHealthComponent::StaticClass()));
	if (HealthComp)
	{
		return HealthComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	}

	return false;
}