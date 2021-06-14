// Copyright Baer and Hoggo Games

#pragma once

#include "EWeapon.generated.h"

/*
* 
*/

UENUM(BlueprintType)
enum class EWeapon : uint8
{
	LaserBlimp		UMETA(DisplayName = "Laser Blimp"),
    MiniGoon		UMETA(DisplayName = "Mini-Goon"),
	Pizdola			UMETA(DisplayName = "Pizdola"),
	Blitz			UMETA(DisplayName = "Blitz"),
	Luncha			UMETA(DisplayName = "Luncha"),
	ShotThy			UMETA(DisplayName = "Shot-Thy"),
	Rak				UMETA(DisplayName = "RAK"),
	AK540			UMETA(DisplayName = "AK-540")

};