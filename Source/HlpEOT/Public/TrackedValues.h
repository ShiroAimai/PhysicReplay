#pragma once

#include "CoreMinimal.h"
#include "TrackedValues.generated.h"

USTRUCT()
struct FReplayValues
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<FVector> VectorValues;
	UPROPERTY(VisibleAnywhere)
	TArray<FQuat> QuatValues;
};

USTRUCT()
struct FReplayRecord
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere)
	TArray<FReplayValues> Record;
};