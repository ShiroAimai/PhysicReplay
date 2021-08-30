// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaCharacter.h"
#include "ArenaEnemy.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AArenaEnemy : public AArenaCharacter
{
	GENERATED_BODY()
public:
	AArenaEnemy();
protected:
	virtual void BeginPlay() override;
	virtual void OnDie() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UAudioComponent* audioComp;
	
	UPROPERTY(EditAnywhere, Category = "Damage")
		float ExplosionDamage = 25.f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere, Category = "Damage")
	USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere, Category = "Configuration")
	UMaterialInterface* Material = nullptr;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
};
