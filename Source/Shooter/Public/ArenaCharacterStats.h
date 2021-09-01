// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArenaCharacterStats.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnStatChanged, AActor*, InstigatorActor, FName, PropertyName, float, NewValue, float, Delta);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UArenaCharacterStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UArenaCharacterStats();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	bool ApplyStatChange(AActor* InstigatorActor, const FString& StatName, float Delta);

	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnStatChanged;
protected:
	UFUNCTION()
	void MulticastStatsChanged(AActor* InstigatorActor, FName PropertyName, float NewValue, float Delta);

	UPROPERTY(EditDefaultsOnly, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float Health = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats", meta = (ClampMin = "0.1", ClampMax = "20.0", UIMin = "0.1", UIMax = "20.0"))
	float Speed = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float Damage = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Stats", meta = (ClampMin = "0.1", ClampMax = "10.0", UIMin = "0.1", UIMax = "10.0"))
	float FireRate = 0.0f;
};
