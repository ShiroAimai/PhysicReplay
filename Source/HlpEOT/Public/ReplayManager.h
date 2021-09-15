// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplayState.h"
#include "TrackedValues.h"
#include "ReplayManager.generated.h"

/**
 * 
 */

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Replay Configuration")
	float SampleRatePerSecond;
	/** Timer Minutes */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Replay Configuration", meta = (ClampMin = "0", ClampMax = "59", UIMin = "00", UIMax = "59"))
	int Minutes;
	/** Timer Seconds */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Replay Configuration", meta = (ClampMin = "0", ClampMax = "59", UIMin = "00", UIMax = "59"))
	int Seconds;
private:
	void EntityDrivenExecute(float deltaTime);
	void ReplayDrivenExecute(float deltaTime);

	void ExtractReplayDataFromComponents();
	void SetReplayDataToComponents(bool& IsReplayFinished);

	TArray<UEntityReplayComponent*> ReplayComponents;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TMap<FName, FReplayRecord> ReplayRecords;

	EReplayState State;
	float elapsed;
	float trackingInterval;
};
