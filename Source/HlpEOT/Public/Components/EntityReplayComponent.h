// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EntityReplayComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HLPEOT_API UEntityReplayComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEntityReplayComponent();
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void GetReplayData(const FString& RequestedTrackProperties, TArray<FVector>& OutTrackPropertiesValues);
	void SetReplayData(const TArray<FVector>& spec) const;

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Replay")
	FVector ReplayLocation;
	UPROPERTY(VisibleAnywhere, Category = "Replay")
	FRotator ReplayRotation;
	UPROPERTY(VisibleAnywhere, Category = "Replay")
	FVector ReplayVelocity;
	UPROPERTY(VisibleAnywhere, Category = "Replay")
	FVector ReplayAngularVelocity;

private:
	class UStaticMeshComponent* OwnerMeshComp;
};
