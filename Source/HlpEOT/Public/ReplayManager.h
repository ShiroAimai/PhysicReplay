// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplayManager.generated.h"

/**
 * 
 */


USTRUCT()
struct FReplayValue
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> Value;
};

USTRUCT()
struct FReplayValuesRecord
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<FReplayValue> Record;
};

UENUM(BlueprintType)
enum class EGameState : uint8
{
	PLAYER_DRIVEN UMETA(DisplayName = "Player Driven"),
	REPLAY_DRIVEN UMETA(DisplayName = "Replay Driven")
};

UCLASS()
class HLPEOT_API AReplayManager : public AActor
{
	GENERATED_BODY()

public:
	AReplayManager();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

protected:
	/** Properties need to be written in the format: property_name,anotherproperty_name  */
	UPROPERTY(EditDefaultsOnly, Category = "Replay Configuration")
	FString TrackedProperties;
private:
	TArray<class UEntityReplayComponent*> ReplayComponents;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TMap<FName, FReplayValuesRecord> ReplayRecords;

	EGameState State;
};
