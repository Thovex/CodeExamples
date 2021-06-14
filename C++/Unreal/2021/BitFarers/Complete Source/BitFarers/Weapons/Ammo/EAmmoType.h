// Copyright Baer and Hoggo Games

#pragma once

#include "EAmmoType.generated.h"

/*
* Determines the Ammo Types that are available
*/

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	Light				UMETA(DisplayName = "Light"),
	Medium				UMETA(DisplayName = "Medium"),
	Energy				UMETA(DisplayName = "Energy"),
	Heavy				UMETA(DisplayName = "Heavy")
};
