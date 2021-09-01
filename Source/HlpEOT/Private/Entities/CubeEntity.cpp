// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/CubeEntity.h"
#include <Components/StaticMeshComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/SceneComponent.h>
#include <Components/BoxComponent.h>

// Sets default values
ACubeEntity::ACubeEntity()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));

	CameraSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm Comp"));
	CameraSpringArmComp->SetupAttachment(RootComponent);
	
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Comp"));
	CollisionComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Comp"));
	CameraComp->SetupAttachment(CameraSpringArmComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComponent);

	Speed = 0.f;
}

// Called when the game starts or when spawned
void ACubeEntity::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACubeEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!EntityVelocity.IsZero())
	{
		FVector ActualLocation = GetActorLocation();
		FVector NewLocation =  FMath::Lerp(ActualLocation, ActualLocation + EntityVelocity, DeltaTime);
		SetActorLocation(NewLocation);
	}
}

// Called to bind functionality to input
void ACubeEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACubeEntity::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACubeEntity::MoveRight);
}

void ACubeEntity::MoveForward(float Value)
{
	EntityVelocity.X = FMath::Clamp(Value, -1.0f, 1.0f) * Speed;
}

void ACubeEntity::MoveRight(float Value)
{
	EntityVelocity.Y = FMath::Clamp(Value, -1.0f, 1.0f) * Speed;
}

