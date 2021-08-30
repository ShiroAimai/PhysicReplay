// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArenaCharacter.generated.h"

class AArenaWeapon;
class UArenaCharacterHealthComponent;
class UArenaWorldWidget;



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
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UArenaCharacterHealthComponent* OwningComp, float NewHealth, float Delta);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArenaCharacterHealthComponent* HealthComp;
};
