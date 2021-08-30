// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaWeapon.generated.h"

class USkeletalMeshComponent;
class UArrowComponent;
class AArenaProjectile;

UCLASS()
class SHOOTER_API AArenaWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArenaWeapon();

	UFUNCTION()
		void StartFire();
	UFUNCTION()
		void StopFire();
protected:
	UPROPERTY(EditAnywhere, Category = "Weapon")
		float fireRate;
	UPROPERTY(EditAnywhere, Category = "Weapon")
		FTimerHandle fireTimer;
	
	UPROPERTY(EditAnywhere, Category = "Projectile")
		TSubclassOf<AArenaProjectile> selectedProjectile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	USkeletalMeshComponent* meshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	UArrowComponent* gunDirectionComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
		UAudioComponent* weaponAudioComponent;

	void Fire() const;
private:
	
	bool isFiring = false;

};
