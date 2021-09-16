#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EReplayState : uint8
{
	NONE UMETA(DisplayName "Replay None"),
	ENTITY_DRIVEN UMETA(DisplayName = "Entity Driven"),
	REPLAY_DRIVEN UMETA(DisplayName = "Replay Driven")
};