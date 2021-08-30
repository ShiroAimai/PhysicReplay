// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ArenaGameplayFunctionLibrary.h"

// Sets default values
AArenaProjectile::AArenaProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	sphereComponent->CanCharacterStepUpOn = ECB_No;
	sphereComponent->SetSphereRadius(10.f);
	sphereComponent->SetNotifyRigidBodyCollision(true);
	sphereComponent->SetGenerateOverlapEvents(true);
	sphereComponent->SetCollisionProfileName("ArenaProjectile");
	
	RootComponent = sphereComponent;

	sphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	sphereMesh->SetupAttachment(sphereComponent);
	sphereMesh->SetWorldScale3D(FVector(.5f, .05f, .05f));

	movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	movementComponent->UpdatedComponent = sphereComponent;
	movementComponent->InitialSpeed = 1000.f;
	movementComponent->MaxSpeed = 3000.f;
	movementComponent->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 4.f;

	damage = 12.f;
}

void AArenaProjectile::BeginPlay()
{
	Super::BeginPlay();
	sphereComponent->OnComponentHit.AddDynamic(this, &AArenaProjectile::OnHit);
}


void AArenaProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UArenaGameplayFunctionLibrary::ApplyDamage(GetInstigator(), OtherActor, damage);
	Destroy();
}

