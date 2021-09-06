#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EReplayState : uint8
{
	NONE UMETA(DisplayName "Replay None"),
	PLAYER_DRIVEN UMETA(DisplayName = "Player Driven"),
	REPLAY_DRIVEN UMETA(DisplayName = "Replay Driven")
};