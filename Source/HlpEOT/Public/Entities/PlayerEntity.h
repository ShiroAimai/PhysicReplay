// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/BaseEntity.h"
#include "PlayerEntity.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class HLPEOT_API APlayerEntity : public ABaseEntity
{
	GENERATED_BODY()
public:
	APlayerEntity();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void UpdateEntity(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USpringArmComponent* CameraSpringArmComp;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UCameraComponent* CameraComp;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	FVector EntityVelocity;
};
