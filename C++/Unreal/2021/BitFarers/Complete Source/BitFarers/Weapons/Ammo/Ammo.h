// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "EAmmoType.h"
#include "Ammo.generated.h"

USTRUCT(BlueprintType)
struct FAmmo
{
	GENERATED_BODY()

	FAmmo(): AmmoType(), Current(0), Max(0){}
	FAmmo(const EAmmoType AmmoType, const int32 Current, const int32 Max)
	{
		this->AmmoType = AmmoType;
		this->Current = Current;
		this->Max = Max;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Current;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Max;
};
