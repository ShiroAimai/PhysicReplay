// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/PlayerEntity.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>

APlayerEntity::APlayerEntity()
{
	CameraSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm Comp"));
	CameraSpringArmComp->SetupAttachment(MeshComp);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Comp"));
	CameraComp->SetupAttachment(CameraSpringArmComp);
}

void APlayerEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerEntity::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerEntity::MoveRight);
}

void APlayerEntity::UpdateEntity(float DeltaTime)
{
	if (!EntityVelocity.IsZero())
	{
		MeshComp->AddForce(EntityVelocity, NAME_None, true);
	}
}

void APlayerEntity::MoveForward(float Value)
{
	EntityVelocity.X = FMath::Clamp(Value, -1.0f, 1.0f) * Speed;
}

void APlayerEntity::MoveRight(float Value)
{
	EntityVelocity.Y = FMath::Clamp(Value, -1.0f, 1.0f) * Speed;
}