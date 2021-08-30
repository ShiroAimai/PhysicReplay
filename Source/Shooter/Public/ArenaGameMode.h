// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaGameMode.generated.h"


class AArenaEnemy;
class AArenaEnemySpawnTargetPoint;
/**
 * 
 */
UCLASS()
class SHOOTER_API AArenaGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
		TSubclassOf<AArenaEnemy> enemy;

	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
		TSubclassOf<AArenaEnemySpawnTargetPoint> spawner;

	UPROPERTY(VisibleAnywhere, Category = "Enemy Spawn")
		int32 currentEnemiesInScene = 0;

	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
		int32 maxEnemies = 1;

	UPROPERTY(EditAnywhere, Category = "Enemy Spawn")
		float enemySpawnRate = 1.f;

	UPROPERTY(EditInstanceOnly, Category = "Enemy Spawn")
		TArray<AActor*> enemySpawnPoints;

private:
	FTimerHandle timer;
	void TryToSpawnEnemies();
	UFUNCTION()
	void OnEnemyDied(AActor* actor);
};
