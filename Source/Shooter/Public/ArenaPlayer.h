// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArenaCharacter.h"
#include "ArenaPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;

DECLARE_DYNAMIC_DELEGATE(FOnFireStart);
DECLARE_DYNAMIC_DELEGATE(FOnFireStop);
/**
 * 
 */
UCLASS()
class SHOOTER_API AArenaPlayer : public AArenaCharacter
{
	GENERATED_BODY()

public:
	AArenaPlayer();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
		FOnFireStart onFireStart;
	UPROPERTY()
		FOnFireStop onFireStop;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Gear")
		TSubclassOf<AArenaWeapon> selectedWeapon;

	void Fire();
	void CeaseFire();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* SpringArmComponent;


	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookAtMouseCursor();

private:
	AArenaWeapon* Weapon = nullptr;
};
