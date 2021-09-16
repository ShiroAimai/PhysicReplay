// Fill out your copyright notice in the Description page of Project Settings.


#include "Entities/ScriptedEntity.h"
#include "ReplayLogContext.h"

AScriptedEntity::AScriptedEntity()
{
	ScriptIndex = 0;
	CurrentIndexDuration = 0.f;
}

void AScriptedEntity::BeginPlay()
{
	Super::BeginPlay();

	if (Scripts.Num() > 0 && ScriptIndex < Scripts.Num())
	{
		CurrentIndexDuration = Scripts[ScriptIndex].DurationInSeconds;
	}
}

void AScriptedEntity::UpdateEntity(float DeltaTime)
{
	if (Scripts.Num() == 0)
	{
		UE_LOG(LogReplay, Warning, TEXT("No scripts added to this entity %s"), *GetDebugName(this));
		return;
	}

	CurrentIndexDuration -= DeltaTime;
	MeshComp->AddForce(Scripts[ScriptIndex].Velocity, NAME_None, true);

	if (CurrentIndexDuration <= 0.f)
	{
		++ScriptIndex;
		if (ScriptIndex >= Scripts.Num())
		{
			ScriptIndex = 0;
		}

		CurrentIndexDuration = Scripts[ScriptIndex].DurationInSeconds;
	}
}
