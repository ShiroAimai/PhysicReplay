// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/BaseEntity.h"
#include "ScriptedEntity.generated.h"

USTRUCT()
struct FScriptedValue
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere)
	FVector Velocity;
	UPROPERTY(EditAnywhere)
	float DurationInSeconds;
};

UCLASS(BlueprintType)
class HLPEOT_API AScriptedEntity : public ABaseEntity
{
	GENERATED_BODY()
public:
	AScriptedEntity();
	virtual void BeginPlay() override;

protected:
	virtual void UpdateEntity(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category = "Script")
	TArray<FScriptedValue> Scripts;

private:
	int ScriptIndex;
	float CurrentIndexDuration;
};
