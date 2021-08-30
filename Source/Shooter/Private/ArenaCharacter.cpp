// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ArenaWeapon.h"
#include "ArenaWorldWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ArenaCharacterHealthComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AArenaCharacter::AArenaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<UArenaCharacterHealthComponent>(TEXT("Health"));
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

	HealthComp->OnHealthChanged.AddDynamic(this, &AArenaCharacter::OnHealthChanged);
}

void AArenaCharacter::OnHealthChanged(AActor* InstigatorActor, UArenaCharacterHealthComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f && NewHealth <= 0.0f)
	{
		OnDie();
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
