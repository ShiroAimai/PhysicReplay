// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplayManager.h"
#include <Kismet/GameplayStatics.h>
#include "Interfaces/ReplayableActor.h"
#include "Components/EntityReplayComponent.h"

AReplayManager::AReplayManager()
{
	PrimaryActorTick.bCanEverTick = true;

	State = EGameState::PLAYER_DRIVEN;
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
}

void AReplayManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(ReplayComponents.Num() == 0) return;

	for (UEntityReplayComponent* ReplayComponent : ReplayComponents)
	{
		FName ComponentName = ReplayComponent->GetFName();
		FReplayValuesRecord& ComponentRecord = ReplayRecords.FindOrAdd(ComponentName);
		FReplayValue NewTrackedValue;
		ReplayComponent->GetReplayData(TrackedProperties, NewTrackedValue.Value);
		ComponentRecord.Record.Add(NewTrackedValue);
	}
}
