// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaEnemy.h"
#include "Components/CapsuleComponent.h"
#include "ArenaGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


AArenaEnemy::AArenaEnemy()
{
	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Enemy Audio Component"));
	audioComp->bStopWhenOwnerDestroyed = true;
	audioComp->SetupAttachment(RootComponent);
}

void AArenaEnemy::BeginPlay()
{
	Super::BeginPlay();
	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(Material, this);
	GetMesh()->SetMaterial(0, DynMaterial);

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AArenaEnemy::OnHit);

	audioComp->Play();
}

void AArenaEnemy::OnDie()
{
	Super::OnDie();
	audioComp->Stop();
}

void AArenaEnemy::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* player = GetWorld()->GetFirstPlayerController()->GetPawn();

	if(player == OtherActor)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
		UArenaGameplayFunctionLibrary::ApplyDamage(GetInstigator(), OtherActor, ExplosionDamage);
		Destroy();
	}
}
