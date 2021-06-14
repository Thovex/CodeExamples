// Copyright Baer and Hoggo Games

#pragma once

#include "EElementalType.generated.h"

/*
* Determines how the firing works, does it shoot one bullet per trigger? Does it shoot multiple rounds (burst)
* or does it automatically shoot at anything.
*/

UENUM(BlueprintType)
enum class EElementalType : uint8
{
	Physical			UMETA(DisplayName = "Physical"),
    Fire				UMETA(DisplayName = "Fire"),
    Ice					UMETA(DisplayName = "Ice"),
	Poison				UMETA(DisplayName = "Poison"),
	Explosive			UMETA(DisplayName = "Explosive"),
	Energy				UMETA(DisplayName = "Energy"),
	Radioactive			UMETA(DisplayName = "Radioactive"),
};