#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "BitFarers/Weapons/WeaponStats.h"

#include "DropLibrary.generated.h"


class AItemPickup;
class UDataTable;
class APlayerCharacter;

UCLASS( Blueprintable )
class UDropLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject")) 
	static void RequestGenericItemDrop(
		const UObject* WorldContextObject, FName Requester, int32 LootShots, TArray<FItemData>& LootItem, TArray<int32>& LootCount);

	/*
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void RequestGenericWeaponDrop(const UObject* WorldContextObject, const TArray<FWeaponStats&>& WeaponItem);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject")) 
	static void SpawnGenericWeaponDrop(const UObject* WorldContextObject, FVector CurrentLocation, const TArray<FWeaponStats>& WeaponItem);
*/
	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void SpawnGenericItemDrop(const UObject* WorldContextObject, FVector CurrentLocation,
		TArray<FItemData> LootItem, TArray<int32> LootCount);


};
