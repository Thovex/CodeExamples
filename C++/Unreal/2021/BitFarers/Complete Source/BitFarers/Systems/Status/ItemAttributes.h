// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"

#include "BitFarers/Systems/Status/Traits/TraitCollection.h"

#include "ItemAttributes.generated.h"

USTRUCT(BlueprintType)
struct FItemAttributes
{
	GENERATED_BODY()

	FItemAttributes() {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTraitCollection TraitCollection;

};
