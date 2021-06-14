// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"

#include "ImpactType.h"
#include "Engine/DataTable.h"
#include "HitLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BITFARERS_API UHitLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static void CreateParticleAndDecal(const UObject* WorldContextObject, FVector Location, FVector Normal,
	                                   USceneComponent* HitComponent, FImpactRow* ImpactRow);
	
	UFUNCTION( BlueprintCallable, Category = "Hit Library", meta = ( WorldContext = "WorldContextObject" ) )
	static void SpawnHitEmitter(const UObject* WorldContextObject, const UDataTable* DT,
	                            const FVector Location, const FVector Normal, USceneComponent* HitComponent);

	UFUNCTION( BlueprintCallable, BlueprintPure, Category = "Hit Library")
	static FVector CalculateNormalFromDirections( const FVector V1, const FVector V2);

};
