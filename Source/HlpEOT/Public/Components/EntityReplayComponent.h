// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReplayState.h"
#include "EntityReplayComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HLPEOT_API UEntityReplayComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEntityReplayComponent();
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void GetReplayData(const FString& RequestedTrackProperties, TArray<FVector>& OutTrackPropertiesValues);
	void SetReplayData(const FString& ReplayProperties, const TArray<FVector>& Specs);

	void UpdateReplayStateWith(EReplayState NewState);

	inline bool IsEntityDriven() const {
		return ReplayState == EReplayState::ENTITY_DRIVEN;
	}
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
	EReplayState ReplayState;
	struct FBodyInstance* OwnerBI;
	
	TArray<FName> AppliedPropertiesNames;

	/**RequestedProperties should be split based on ","*/
	void ExtractTrackedProperties(const FString& RequestedProperties, TArray<FString>& OutSplittedProperties) const;
	FStructProperty* RetrieveStructPropertyByString(const FString& PropertyName, void*& OutPropertyAddressInObject);
	FProperty* GetLocalPropertyByString(const FString& PropertyName, void*& OutObject);
};
