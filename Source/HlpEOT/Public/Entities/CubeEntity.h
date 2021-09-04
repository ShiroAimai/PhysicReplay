// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/ReplayableActor.h"
#include "CubeEntity.generated.h"

class UEntityReplayComponent;

UCLASS()
class HLPEOT_API ACubeEntity : public APawn, public IReplayableActor
{
	GENERATED_BODY()

public:
	ACubeEntity();

protected:
	
	UPROPERTY(EditAnywhere, Category = "Entity")
	float Speed;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USpringArmComponent* CameraSpringArmComp;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UEntityReplayComponent* ReplayComp;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UEntityReplayComponent* GetReplayComponentFromActor() override;
private:

	void MoveForward(float Value);
	void MoveRight(float Value);

	FVector EntityVelocity;
};
