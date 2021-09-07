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

	UFUNCTION(BlueprintImplementableEvent)
	void OnReplayedAllEntities();
protected:
	/** Properties need to be written in the format: property_name,anotherproperty_name  */
	UPROPERTY(EditDefaultsOnly, Category = "Replay Configuration")
	FString TrackedProperties;

	/** Timer Minutes */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Replay Configuration", meta = (ClampMin = "0", ClampMax = "59", UIMin = "00", UIMax = "59"))
	int Minutes;
	/** Timer Seconds */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Replay Configuration", meta = (ClampMin = "0", ClampMax = "59", UIMin = "00", UIMax = "59"))
	int Seconds;
private:
	void ExtractReplayDataFromComponents();
	void SetReplayDataToComponents(bool& IsReplayFinished);

	TArray<UEntityReplayComponent*> ReplayComponents;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TMap<FName, FReplayRecord> ReplayRecords;

	EReplayState State;
	/** Index used in state REPLAY_DRIVEN to sequentially iterate on tracked values */
	size_t ReplayIndex = 0;
};
