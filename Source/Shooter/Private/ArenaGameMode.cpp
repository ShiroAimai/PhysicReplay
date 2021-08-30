// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaGameMode.h"
#include "ArenaEnemy.h"
#include "ArenaEnemySpawnTargetPoint.h"
#include "Kismet/GameplayStatics.h"

void AArenaGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), spawner.Get(), enemySpawnPoints);

	if (enemySpawnPoints.Num() == 0) return;

	GetWorldTimerManager()
		.SetTimer(timer, this, &AArenaGameMode::TryToSpawnEnemies, enemySpawnRate, true, enemySpawnRate);
}

void AArenaGameMode::TryToSpawnEnemies()
{
	if (currentEnemiesInScene == maxEnemies) return;

	for (int i = 0; i < enemySpawnPoints.Num() && currentEnemiesInScene < maxEnemies; ++i)
	{
		AArenaEnemy* spawnedEnemy = GetWorld()->SpawnActor<AArenaEnemy>(enemy.Get(), enemySpawnPoints[i]->GetTransform());
		currentEnemiesInScene++;

		spawnedEnemy->OnDestroyed.AddDynamic(this, &AArenaGameMode::OnEnemyDied);
	}
}

void AArenaGameMode::OnEnemyDied(AActor* actor) {
	currentEnemiesInScene--;
}
