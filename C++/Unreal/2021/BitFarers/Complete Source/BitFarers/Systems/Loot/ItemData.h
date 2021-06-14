// Copyright Baer and Hoggo Games

#pragma once

#include "Engine/DataTable.h"

#include "ItemData.generated.h"

class ASlurpPickup;

UENUM(BlueprintType)
enum class EItemCategory : uint8 
{
	Food		UMETA(DisplayName = "Food"),
    Cosmetics	UMETA(DisplayName = "Cosmetics"),
    Valuables	UMETA(DisplayName = "Valuables"),
    Ammunition	UMETA(DisplayName = "Ammo"),
    Crafting	UMETA(DisplayName = "Crafting"),
    Junk		UMETA(DisplayName = "Junk")
};

UENUM(BlueprintType)
enum class EItemRarity : uint8 
{
	Scrappy		UMETA(DisplayName = "Scrappy"),
    Common		UMETA(DisplayName = "Common"),
    Uncommon	UMETA(DisplayName = "Uncommon"),
    Rare		UMETA(DisplayName = "Rare"),
    Unique		UMETA(DisplayName = "Unique")
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
public:

	/*
	*	W	A	R	N	I	N	G 
	*
	*	If you make a new variable in the item struct:
	*	Modify it in the DropLibrary.h struct too! Add it to the constructor.
	*  
	*	W	A	R	N	I	N	G 
	*/
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemCategory ItemCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemRarity ItemRarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemStackCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<USkeletalMesh*> ItemMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UParticleSystem* ItemParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<ASlurpPickup> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 DataInt;

};

USTRUCT(BlueprintType)
struct FDropItemData : public FTableRowBase
{
	GENERATED_BODY()
public:
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ItemDropPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<int32> ItemDropCounts;
};

USTRUCT(BlueprintType)
struct FTableLinker : public FTableRowBase
{
	GENERATED_BODY()
public:
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ObjectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UDataTable* AssociatedDropTable;

};
