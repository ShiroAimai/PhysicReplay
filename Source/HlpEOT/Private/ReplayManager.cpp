// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplayManager.h"
#include <Kismet/GameplayStatics.h>
#include "Interfaces/ReplayableActor.h"
#include "Components/EntityReplayComponent.h"
#include "ReplayLogContext.h"

#pragma optimize("",off)
AReplayManager::AReplayManager()
{
	PrimaryActorTick.bCanEverTick = true;

	SampleRatePerSecond = 1.f;
	trackingInterval = 1.f;
	elapsed = 0.f;
	State = EReplayState::ENTITY_DRIVEN;
}

void AReplayManager::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UReplayableActor::StaticClass(), OutActors);

	for (AActor* TrackedActor : OutActors)
	{
		IReplayableActor* ReplayableActor = Cast<IReplayableActor>(TrackedActor);
		if (ReplayableActor)
		{
			ReplayComponents.Add(ReplayableActor->GetReplayComponentFromActor());
		}
	}

	if(ReplayComponents.Num() > 0)
	{
		for (UEntityReplayComponent* Comp : ReplayComponents)
			Comp->UpdateReplayStateWith(State);
	}
	else
	{
		UE_LOG(LogReplay, Warning, TEXT("No ReplayComponents found"));
	}

	trackingInterval = 1.f / SampleRatePerSecond;
	elapsed = trackingInterval;
}

void AReplayManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ReplayComponents.Num() == 0) return;

	if (State == EReplayState::ENTITY_DRIVEN)
	{
		EntityDrivenExecute(DeltaSeconds);
	}
	else
	{
		ReplayDrivenExecute(DeltaSeconds);
	}
}

void AReplayManager::SetState(EReplayState NewState)
{
	if(State == NewState) return;
	State = NewState;
	
	if (State == EReplayState::ENTITY_DRIVEN)
	{
		elapsed = 0.f;
	}

	for (UEntityReplayComponent* ReplayComponent : ReplayComponents)
	{
		ReplayComponent->UpdateReplayStateWith(State);
	}
}

void AReplayManager::EntityDrivenExecute(float deltaTime)
{
	elapsed += deltaTime;
	if (elapsed >= trackingInterval)
	{
		ExtractReplayDataFromComponents();
		elapsed = 0.f;
	}
}

void AReplayManager::ReplayDrivenExecute(float deltaTime)
{
	elapsed += deltaTime;

	bool isReplayFinished = false;
	SetReplayDataToComponents(isReplayFinished);

	if (isReplayFinished)
	{
		OnReplayedAllEntities();
	}
}

void AReplayManager::ExtractReplayDataFromComponents()
{
	for (UEntityReplayComponent* ReplayComponent : ReplayComponents)
	{
		FName ComponentName = ReplayComponent->GetOwner()->GetFName();
		FReplayRecord& ComponentRecord = ReplayRecords.FindOrAdd(ComponentName);
		TArray<FVector> NewTrackedValues;
		ReplayComponent->GetReplayData(TrackedProperties, NewTrackedValues);
		for (size_t i = 0; i < NewTrackedValues.Num(); i++)
		{
			if (ComponentRecord.Record.Num() <= i)
			{
				ComponentRecord.Record.Add(FReplayValues());
			}
			ComponentRecord.Record[i].Values.Add(NewTrackedValues[i]);
		}
	}
}

void AReplayManager::SetReplayDataToComponents(bool& IsReplayFinished)
{
	const size_t ReplayIndex = FMath::Floor(elapsed / trackingInterval);
	const float t_ReplayIndex = ReplayIndex * trackingInterval;
	const size_t NextReplayIndex = ReplayIndex + 1;
	const float t_NextReplayIndex = NextReplayIndex * trackingInterval;

	const float InterpFactor = (elapsed - t_ReplayIndex) / (t_NextReplayIndex - t_ReplayIndex);

	for (UEntityReplayComponent* ReplayComponent : ReplayComponents)
	{
		FName ComponentName = ReplayComponent->GetOwner()->GetFName();
		const FReplayRecord* ComponentRecord = ReplayRecords.Find(ComponentName);
		if(!ComponentRecord) continue;

		TArray<FVector> ReplayValuesForIndex;
		for (const FReplayValues& ReplayValue : ComponentRecord->Record)
		{
			if (ReplayIndex >= ReplayValue.Values.Num() || NextReplayIndex >= ReplayValue.Values.Num())
			{
				if (!IsReplayFinished)
				{
					IsReplayFinished = true;
				}
				ReplayComponent->UpdateReplayStateWith(EReplayState::NONE);
				break;
			}
			FVector LerpValue = FMath::VInterpConstantTo(ReplayValue.Values[ReplayIndex], ReplayValue.Values[NextReplayIndex], InterpFactor, 1.f);
			ReplayValuesForIndex.Add(LerpValue);
		}
		ReplayComponent->SetReplayData(TrackedProperties, ReplayValuesForIndex);
	}
}
#pragma optimize("",on)