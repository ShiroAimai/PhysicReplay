// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplayManager.h"
#include <Kismet/GameplayStatics.h>
#include "Interfaces/ReplayableActor.h"
#include "Components/EntityReplayComponent.h"

AReplayManager::AReplayManager()
{
	PrimaryActorTick.bCanEverTick = true;

	State = EReplayState::PLAYER_DRIVEN;
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

	for (UEntityReplayComponent* Comp : ReplayComponents)
		Comp->UpdateReplayStateWith(State);

}

void AReplayManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ReplayComponents.Num() == 0) return;

	if (State == EReplayState::PLAYER_DRIVEN)
	{
		ExtractReplayDataFromComponents();
	}
	else
	{
		SetReplayDataToComponents();	
		++ReplayIndex;
	}
}

void AReplayManager::SetState(EReplayState NewState)
{
	if(State == NewState) return;
	State = NewState;
	
	if (State == EReplayState::PLAYER_DRIVEN)
	{
		ReplayIndex = 0;
	}

	for (UEntityReplayComponent* ReplayComponent : ReplayComponents)
	{
		ReplayComponent->UpdateReplayStateWith(State);
	}
}

void AReplayManager::ExtractReplayDataFromComponents()
{
	for (UEntityReplayComponent* ReplayComponent : ReplayComponents)
	{
		FName ComponentName = ReplayComponent->GetFName();
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

void AReplayManager::SetReplayDataToComponents()
{
	for (UEntityReplayComponent* ReplayComponent : ReplayComponents)
	{
		FName ComponentName = ReplayComponent->GetFName();
		FReplayRecord& ComponentRecord = ReplayRecords.FindOrAdd(ComponentName);
		TArray<FVector> ReplayValuesForIndex;
		for (const FReplayValues& ReplayValue : ComponentRecord.Record)
		{
			if (ReplayIndex >= ReplayValue.Values.Num()) break;
			ReplayValuesForIndex.Add(ReplayValue.Values[ReplayIndex]);
		}
		ReplayComponent->SetReplayData(TrackedProperties, ReplayValuesForIndex);
	}
}
