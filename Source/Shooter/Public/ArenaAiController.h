// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ArenaAiController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AArenaAiController : public AAIController
{
	GENERATED_BODY()

public:
	AArenaAiController();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ai Config")
	FTimerHandle chaseTimer;

	UPROPERTY(EditAnywhere, Category = "Ai Config")
	float chaseRateTime;

private:
	void ChasePlayer();
	UPROPERTY(VisibleAnywhere, Category = "Ai Chase")
	APawn* playerToChase = nullptr;
};
