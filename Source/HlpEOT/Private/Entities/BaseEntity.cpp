// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/BaseEntity.h"
#include <Components/StaticMeshComponent.h>
#include "Components/EntityReplayComponent.h"

// Sets default values
ABaseEntity::ABaseEntity()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	RootComponent = MeshComp;

	ReplayComp = CreateDefaultSubobject<UEntityReplayComponent>(TEXT("Entity Replay Component"));

	Speed = 0.f;
}

void ABaseEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!ReplayComp->IsEntityDriven()) return;

	UpdateEntity(DeltaTime);
}

UEntityReplayComponent* ABaseEntity::GetReplayComponentFromActor()
{
	return ReplayComp;
}
