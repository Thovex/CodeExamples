// Copyright Baer and Hoggo Games

#pragma once

#include "EFiringStyle.generated.h"

/*
 * Determines what comes out of the gun. E.g. Is a gun HitScan, does a gun fire projectiles?
 */

UENUM(BlueprintType)
enum class EFiringStyle : uint8
{
	HitScan			UMETA(DisplayName = "HitScan"),
    Projectile		UMETA(DisplayName = "Projectile"),
};