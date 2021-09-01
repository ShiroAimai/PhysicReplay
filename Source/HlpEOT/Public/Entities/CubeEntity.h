// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CubeEntity.generated.h"

UCLASS()
class HLPEOT_API ACubeEntity : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACubeEntity();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Entity")
	float Speed;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UBoxComponent* CollisionComp;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USpringArmComponent* CameraSpringArmComp;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UCameraComponent* CameraComp;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void MoveForward(float Value);
	void MoveRight(float Value);

	FVector EntityVelocity;

};
