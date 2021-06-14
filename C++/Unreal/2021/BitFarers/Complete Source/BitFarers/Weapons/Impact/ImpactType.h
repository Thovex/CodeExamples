// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NiagaraSystem.h"
#include "ImpactType.generated.h"


USTRUCT(BlueprintType)
struct FImpactRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ImpactSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* NImpactSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ImpactSystemScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector NImpactSystemScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* ImpactDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ImpactDecalScale = 8;
};	