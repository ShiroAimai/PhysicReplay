// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplayState.h"
#include "ReplayManager.generated.h"

/**
 * 
 */


USTRUCT()
struct FReplayValues
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> Values;
};

USTRUCT()
struct FReplayRecord
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<FReplayValues> Record;
};

class UEntityReplayComponent;

UCLASS()
class HLPEOT_API AReplayManager : public AActor
{
	GENERATED_BODY()

public:
	AReplayManager();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
	UFUNCTION(BlueprintCallable)
	void SetState(EReplayState NewState);

protected:
	/** Properties need to be written in the format: property_name,anotherproperty_name  */
	UPROPERTY(EditDefaultsOnly, Category = "Replay Configuration")
	FString TrackedProperties;

private:
	void ExtractReplayDataFromComponents();
	void SetReplayDataToComponents();

	TArray<UEntityReplayComponent*> ReplayComponents;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TMap<FName, FReplayRecord> ReplayRecords;

	EReplayState State;
	size_t ReplayIndex = 0;
};
