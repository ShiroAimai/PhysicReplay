// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/ReplayableActor.h"
#include "BaseEntity.generated.h"

class UEntityReplayComponent;

UCLASS(Abstract)
class HLPEOT_API ABaseEntity : public APawn, public IReplayableActor
{
	GENERATED_BODY()

public:
	ABaseEntity();
	virtual void Tick(float DeltaTime) override;

	virtual UEntityReplayComponent* GetReplayComponentFromActor() override;
protected:
	virtual void UpdateEntity(float DeltaTime) PURE_VIRTUAL(ABaseEntity::UpdateEntity(DeltaTime));
	
	UPROPERTY(EditAnywhere, Category = "Entity")
	float Speed;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UEntityReplayComponent* ReplayComp;
};
