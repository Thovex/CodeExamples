#pragma once

#include "ModifierType.h"
#include "TraitModifier.generated.h"

USTRUCT(BlueprintType)
struct FTraitModifier
{
	GENERATED_BODY()

public:
	FTraitModifier(): Power(0), Type(EModifierType::Additive), Duration(0)
	{
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trait|Modifier")
	FGuid GUID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trait|Modifier")
	FName Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trait|Modifier")
	float Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trait|Modifier")
	EModifierType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trait|Modifier")
	float Duration;
};