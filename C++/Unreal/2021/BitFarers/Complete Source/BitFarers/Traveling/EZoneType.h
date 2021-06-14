// Copyright Baer and Hoggo Games

#pragma once

#include "EZoneType.generated.h"

/*
* 
*/

UENUM(BlueprintType)
enum class EZoneType : uint8
{
	ActBegin	UMETA(DisplayName = "Act Begin"),
	Puzzle		UMETA(DisplayName = "Puzzle"),
	Combat		UMETA(DisplayName = "Combat"),
	Event		UMETA(DisplayName = "Event"),
	Merchants	UMETA(DisplayName = "Merchant"),
	Resting		UMETA(DisplayName = "Resting"),
	Lottery		UMETA(DisplayName = "Lottery"),
	Boss		UMETA(DisplayName = "Boss"),
	Random		UMETA(DisplayName = "Random"),
	Unknown		UMETA(DisplayName = "Unknown"),
};


ENUM_RANGE_BY_COUNT(EZoneType, EZoneType::Unknown)
