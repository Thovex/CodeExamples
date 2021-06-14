// Copyright Baer and Hoggo Games

#pragma once

#include "EFiringMode.generated.h"

/*
* Determines how the firing works, does it shoot one bullet per trigger? Does it shoot multiple rounds (burst)
* or does it automatically shoot at anything.
*/

UENUM(BlueprintType)
enum class EFiringMode : uint8
{
	Single			UMETA(DisplayName = "Single"),
    Burst			UMETA(DisplayName = "Burst"),
	Auto			UMETA(DisplayName = "Auto"),
};