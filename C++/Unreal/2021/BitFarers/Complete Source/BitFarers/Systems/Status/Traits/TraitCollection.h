// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"


#include "ETraitType.h"
#include "TraitCollection.generated.h"

USTRUCT(BlueprintType)
struct FTraitCollection
{
	GENERATED_BODY()

	FTraitCollection()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETraitType, float> Traits;

};
