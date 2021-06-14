// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BitFarers/Weapons/Ammo/AmmoComponent.h"
#include "AmmoInterface.generated.h"

UINTERFACE(BlueprintType)
class BITFARERS_API UAmmoInterface : public UInterface
{
	GENERATED_BODY()

	UAmmoInterface();
};

/**
* 
*/
class BITFARERS_API IAmmoInterface
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "HasStatus Interface")
	UAmmoComponent* RetrieveAmmoComponent();
};
