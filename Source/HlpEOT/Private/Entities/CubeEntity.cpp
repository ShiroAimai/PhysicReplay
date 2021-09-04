// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/CubeEntity.h"
#include <Components/StaticMeshComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/SceneComponent.h>
#include <Components/BoxComponent.h>
#include "Components/EntityReplayComponent.h"

// Sets default values
ACubeEntity::ACubeEntity()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	RootComponent = MeshComp;

	CameraSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm Comp"));
	CameraSpringArmComp->SetupAttachment(MeshComp);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Comp"));
	CameraComp->SetupAttachment(CameraSpringArmComp);

	ReplayComp = CreateDefaultSubobject<UEntityReplayComponent>(TEXT("Entity Replay Component"));

	Speed = 0.f;
}


// Called every frame
void ACubeEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!EntityVelocity.IsZero())
	{
		MeshComp->AddForce(EntityVelocity, NAME_None, true);
	}
}

// Called to bind functionality to input
void ACubeEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACubeEntity::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACubeEntity::MoveRight);
}

UEntityReplayComponent* ACubeEntity::GetReplayComponentFromActor()
{
	return ReplayComp;
}

void ACubeEntity::MoveForward(float Value)
{
	EntityVelocity.X = FMath::Clamp(Value, -1.0f, 1.0f) * Speed;
}

void ACubeEntity::MoveRight(float Value)
{ 
	EntityVelocity.Y = FMath::Clamp(Value, -1.0f, 1.0f) * Speed;
}

