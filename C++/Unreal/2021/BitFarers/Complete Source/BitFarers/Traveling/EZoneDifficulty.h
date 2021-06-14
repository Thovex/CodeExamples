// Copyright Baer and Hoggo Games

#pragma once

#include "EZoneDifficulty.generated.h"

/*
* 
*/

UENUM(BlueprintType)
enum class EZoneDifficulty : uint8
{
	Normal			UMETA(DisplayName = "Normal"),
    Medium			UMETA(DisplayName = "Medium"),
	Hard			UMETA(DisplayName = "Hard"),
	Elite			UMETA(DisplayName = "Elite"),

};