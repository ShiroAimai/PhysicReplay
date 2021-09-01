// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ArenaWeapon.h"
#include "ArenaWorldWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ArenaCharacterStats.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AArenaCharacter::AArenaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Stats = CreateDefaultSubobject<UArenaCharacterStats>(TEXT("Character Stats"));
}

// Called when the game starts or when spawned
void AArenaCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (ActiveHealthBar == nullptr)
	{
		ActiveHealthBar = CreateWidget<UArenaWorldWidget>(GetWorld(), HealthBarWidgetClass);
		if (ActiveHealthBar)
		{
			ActiveHealthBar->AttachedActor = this;
			ActiveHealthBar->AddToViewport();
		}
	}
}

void AArenaCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if(ObservedStats.Num() > 0)
	{
		Stats->OnStatChanged.AddDynamic(this, &AArenaCharacter::OnStatChanged);
	}
}

void AArenaCharacter::OnStatChanged(AActor* InstigatorActor, FName PropertyName, float NewValue, float Delta)
{
	if (ObservedStats.Contains(PropertyName))
	{
		
	}
}

void AArenaCharacter::OnDie()
{
	// ragdoll
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	// set lifespan
	SetLifeSpan(1.5f);
}
