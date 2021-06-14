// Copyright Baer and Hoggo Games

#pragma once

#include "EFiringType.generated.h"

/*
* Hold to shoot or click to shoot.
*/


UENUM(BlueprintType)
enum class EFiringType : uint8
{
	Automatic		UMETA(DisplayName = "Automatic Hold"),
    Single			UMETA(DisplayName = "Single Tap"),
};