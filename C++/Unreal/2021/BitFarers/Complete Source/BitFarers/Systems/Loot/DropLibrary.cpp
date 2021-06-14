#include "DropLibrary.h"


#include "ItemData.h"
#include "BitFarers/Macros.h"
#include "BitFarers/Systems/NetworkGameState.h"
#include "BitFarers/Utilities/AccessorLibrary.h"
#include "BitFarers/Weapons/WeaponStats.h"
#include "Engine/DataTable.h"
#include "Pickups/Clientside/ClientItemPickup.h"
#include "Pickups/Clientside/ClientWeaponPickup.h"

void UDropLibrary::RequestGenericItemDrop(
	const UObject* WorldContextObject, FName Requester, int32 LootShots, TArray<FItemData>& LootItem,
	TArray<int32>& LootCount)
{
	ANetworkGameState* NetworkGameState = UAccessorLibrary::GetNetworkGameState(WorldContextObject);

	UDataTable* LootTable = NetworkGameState->LootTable;
	UDataTable* AssociationTable = NetworkGameState->AssociationTable;

	RETURN_IF_NULLPTR(LootTable);
	RETURN_IF_NULLPTR(AssociationTable);

	FString RequesterWarning;
	FTableLinker* TableLinker = AssociationTable->FindRow<FTableLinker>(Requester, RequesterWarning, false);

	RETURN_IF_NULLPTR(TableLinker);

	UDataTable* DropTable = TableLinker->AssociatedDropTable;
	RETURN_IF_NULLPTR(DropTable);

	for (int32 i = 0; i < LootShots; i++)
	{
		for (auto& DropRow : DropTable->GetRowNames())
		{
			FDropItemData* DropTableItem = DropTable->FindRow<FDropItemData>(DropRow, RequesterWarning, false);
			RETURN_IF_NULLPTR(DropTableItem);
			
			if (FMath::RandRange(0.0f, 100.0f) <= DropTableItem->ItemDropPercentage)
			{
				FItemData* ItemData = LootTable->FindRow<FItemData>(DropTableItem->ItemName, RequesterWarning, false);
				RETURN_IF_NULLPTR(ItemData);

				FItemData CalculatedItemData = FItemData();
				CalculatedItemData.ItemName = ItemData->ItemName;
				CalculatedItemData.ItemCategory = ItemData->ItemCategory;
				CalculatedItemData.ItemRarity = ItemData->ItemRarity;
				CalculatedItemData.ItemStackCount = ItemData->ItemStackCount;
				CalculatedItemData.ItemIcon = ItemData->ItemIcon;
				CalculatedItemData.ItemMeshes = ItemData->ItemMeshes;
				CalculatedItemData.ItemParticle = ItemData->ItemParticle;
				CalculatedItemData.ItemClass = ItemData->ItemClass;
				CalculatedItemData.DataInt = ItemData->DataInt;

				LootItem.Add(CalculatedItemData);
				LootCount.Add(DropTableItem->ItemDropCounts
					[FMath::RandRange(0, DropTableItem->ItemDropCounts.Num() - 1)]);

				break;
			}
		}
	}
}

/*
void UDropLibrary::RequestGenericWeaponDrop(const UObject* WorldContextObject, const TArray<FWeaponStats&>& WeaponItem)
{
	ANetworkGameState* NetworkGameState = UAccessorLibrary::GetNetworkGameState(WorldContextObject);

	UDataTable* WeaponTable = NetworkGameState->WeaponTable;

	RETURN_IF_NULLPTR(WeaponTable);

	FString RequesterWarning;
	TArray<FWeaponStats*> Rows;
	WeaponTable->GetAllRows<FWeaponStats>(RequesterWarning, Rows);

	if (Rows.Num() > 0)
	{
		TArray<FWeaponStats*> RandomWeapons = Rows[FMath::RandRange(0, Rows.Num() - 1)];
		RETURN_IF_NULLPTR(RandomWeapon);

		FWeaponStats CalculatedWeaponStats = FWeaponStats();
		CalculatedWeaponStats.bAutoReload = RandomWeapon->bAutoReload;

		CalculatedWeaponStats.AmmoType = RandomWeapon->AmmoType;
		CalculatedWeaponStats.ElementalType = RandomWeapon->ElementalType;
		
		CalculatedWeaponStats.FiringMode = RandomWeapon->FiringMode;
		CalculatedWeaponStats.FiringStyle = RandomWeapon->FiringStyle;
		CalculatedWeaponStats.FiringType = RandomWeapon->FiringType;

		CalculatedWeaponStats.SetBaseDamage(RandomWeapon->GetBaseDamage());
		CalculatedWeaponStats.SetBaseRange(RandomWeapon->GetBaseRange());
		CalculatedWeaponStats.SetBaseAmmoUsage(RandomWeapon->GetBaseAmmoUsage()) ;
		CalculatedWeaponStats.SetBaseReloadSpeed(RandomWeapon->GetBaseReloadSpeed());
		CalculatedWeaponStats.SetBaseHitSpread(RandomWeapon->GetBaseHitSpread()) ;
		CalculatedWeaponStats.SetBaseProjectileCount(RandomWeapon->GetBaseProjectileCount());
		CalculatedWeaponStats.SetBaseBurstCount(RandomWeapon->GetBaseBurstCount());

		CalculatedWeaponStats.WeaponClass = RandomWeapon->WeaponClass;
		CalculatedWeaponStats.WeaponMesh = RandomWeapon->WeaponMesh;
		CalculatedWeaponStats.WeaponName = RandomWeapon->WeaponName;

		CalculatedWeaponStats.Attributes = RandomWeapon->Attributes;

		WeaponItem = CalculatedWeaponStats;
	}
}

void UDropLibrary::SpawnGenericWeaponDrop(const UObject* WorldContextObject, FVector CurrentLocation, const TArray<FWeaponStats>& WeaponItem)
{
	UWorld* World = WorldContextObject->GetWorld();
	
	RETURN_IF_NULLPTR(World);

	const FVector SpawnLocation = CurrentLocation + FVector(0, 0, 50);
	FActorSpawnParameters Parameters;
	Parameters.bNoFail = true;
	
	const float RndX = FMath::RandRange(-25.0F, 25.0F);
	const float RndY = FMath::RandRange(-25.0F, 25.0F);
	const float RndZ = FMath::RandRange(-25.0F, 25.0F);

	const float RndRoll = FMath::RandRange(0.0F, 360.0F);

	const FVector ThisSpawnLocation = SpawnLocation + FVector(RndX, RndY, RndZ);
	const FRotator ThisSpawnRotation = FRotator(0, 0, RndRoll);

	auto WeaponPickup = World->SpawnActor<AClientWeaponPickup>(ThisSpawnLocation, ThisSpawnRotation, Parameters);

	RETURN_IF_NULLPTR(WeaponPickup);

	WeaponPickup->InitializeWeapon(WeaponItem);
}

*/

void UDropLibrary::SpawnGenericItemDrop(const UObject* WorldContextObject, FVector CurrentLocation, TArray<FItemData> LootItem,
	TArray<int32> LootCount)
{
	UWorld* World = WorldContextObject->GetWorld();
	
	RETURN_IF_NULLPTR(World);

	const FVector SpawnLocation = CurrentLocation + FVector(0, 0, 50);
	FActorSpawnParameters Parameters;
	Parameters.bNoFail = true;
	
	for (int32 i = 0; i < LootItem.Num(); i++)
	{
		const float RndX = FMath::RandRange(-25.0F, 25.0F);
		const float RndY = FMath::RandRange(-25.0F, 25.0F);
		const float RndZ = FMath::RandRange(-25.0F, 25.0F);

		const float RndRoll = FMath::RandRange(0.0F, 360.0F);

		const FVector ThisSpawnLocation = SpawnLocation + FVector(RndX, RndY, RndZ);
		const FRotator ThisSpawnRotation = FRotator(0, 0, RndRoll);

		auto ItemPickup = World->SpawnActor<AClientItemPickup>(LootItem[i].ItemClass, ThisSpawnLocation, ThisSpawnRotation, Parameters);

		RETURN_IF_NULLPTR(ItemPickup);

		ItemPickup->InitializePickup(LootItem[i], LootCount[i]);
	}
}
