// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArenaCharacter.generated.h"

class AArenaWeapon;
class UArenaCharacterHealthComponent;
class UArenaWorldWidget;
class UArenaCharacterStats;

UCLASS()
class SHOOTER_API AArenaCharacter : public ACharacter
{
	GENERATED_BODY()
		
public:
	// Sets default values for this character's properties
	AArenaCharacter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void OnDie();

	UArenaWorldWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	//UPROPERTY(EditDefaultsOnly, Category = "Stats")
	
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	TSet<FName> ObservedStats;

	UFUNCTION()
	void OnStatChanged(AActor* InstigatorActor, FName PropertyName, float NewValue, float Delta);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArenaCharacterStats* Stats;
};
