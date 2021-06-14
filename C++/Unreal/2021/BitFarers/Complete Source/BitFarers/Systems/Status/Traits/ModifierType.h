#pragma once

#include "ModifierType.generated.h"

UENUM(BlueprintType)
enum class EModifierType : uint8
{
	Additive UMETA(DisplayName = "Additive"),
    Multiplier UMETA(DisplayName = "Multiplier"),
    Divider UMETA(DisplayName = "Divider"),
    Override UMETA(DisplayName = "Override"),
};
