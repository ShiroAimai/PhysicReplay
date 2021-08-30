// Fill out your copyright notice in the Description page of Project Settings.

#include "ArenaWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "ArenaProjectile.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AArenaWeapon::AArenaWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	meshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Skeletal Mesh"));
	RootComponent = meshComponent;

	gunDirectionComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Gun Direction"));
	gunDirectionComponent->SetupAttachment(meshComponent);
	gunDirectionComponent->ArrowSize = .5f;

	FVector gunDirCompLocation = FVector(0.f, 50.f, 10.f);
	FRotator gunDirCompRotation = FVector(0.f, 0.f, 90.f).Rotation();
	gunDirectionComponent->SetRelativeLocationAndRotation(gunDirCompLocation, gunDirCompRotation);

	weaponAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Weapon Audio Component"));
	weaponAudioComponent->bStopWhenOwnerDestroyed = true;
	weaponAudioComponent->SetupAttachment(RootComponent);
}

void AArenaWeapon::StartFire()
{
	if (isFiring) return;
	isFiring = true;
	GetWorldTimerManager().SetTimer(fireTimer, this, &AArenaWeapon::Fire, fireRate, true, 0.f);
}

void AArenaWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(fireTimer);
	isFiring = false;
}

void AArenaWeapon::Fire() const
{
	GetWorld()->SpawnActor<AArenaProjectile>(
		selectedProjectile.Get(),
		gunDirectionComponent->GetComponentTransform());
	weaponAudioComponent->Play();
}