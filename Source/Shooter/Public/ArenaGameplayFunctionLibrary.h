// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ArenaGameplayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UArenaGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		static bool ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount);
};
