// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"

#include "WeaponStats.h"

#include "WeaponShootUtilityLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BITFARERS_API UWeaponShootUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure) 
	static void GetUnitVectors(USceneComponent* Component, FVector& Up, FVector& Right, FVector& Forward);
	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject")) 
	static void GenerateShotRaycastsByStreamCreation(const UObject* WorldContextObject, const float Range,
		const float HitSpread, const FVector Location, const ACharacter* CharacterRef,
		const int32 RandomSeed, const int32 Count, TArray<FVector>& StartPoints, TArray<FVector>& EndPoints);
	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject")) 
	static void GenerateShotRaycasts(const UObject* WorldContextObject, ACharacter* CharacterRef, FWeaponStats WeaponStats,
		USceneComponent* ShootDirection, TArray<FVector>& StartPoints, TArray<FVector>& EndPoints);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject")) 
	static void GenerateShotRaycastsWithOutput(const UObject* WorldContextObject, ACharacter* CharacterRef, FWeaponStats WeaponStats,
    USceneComponent* ShootDirection, TArray<FHitResult>& Results);

	
};
