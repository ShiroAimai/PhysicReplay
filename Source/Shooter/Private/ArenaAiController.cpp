// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaAiController.h"
#include "Kismet/GameplayStatics.h"

AArenaAiController::AArenaAiController()
{
}

void AArenaAiController::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(chaseTimer, this, &AArenaAiController::ChasePlayer, chaseRateTime, true, chaseRateTime);
}

void AArenaAiController::ChasePlayer()
{
	playerToChase = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	auto result = MoveToActor(playerToChase, 5.f, false);
}
