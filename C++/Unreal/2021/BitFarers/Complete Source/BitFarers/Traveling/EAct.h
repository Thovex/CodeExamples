// Copyright Baer and Hoggo Games

#pragma once

#include "EAct.generated.h"

/*
* 
*/

UENUM(BlueprintType)
enum class EAct : uint8
{
	Other			UMETA(DisplayName = "Other"),
	Island			UMETA(DisplayName = "Island"),
    Desert			UMETA(DisplayName = "Desert"),
	Underworld			UMETA(DisplayName = "Underworld"),
};