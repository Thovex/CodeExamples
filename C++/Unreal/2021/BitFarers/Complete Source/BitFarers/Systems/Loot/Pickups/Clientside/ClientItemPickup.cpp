// Copyright Baer and Hoggo Games


#include "ClientItemPickup.h"
#include "BitFarers/Macros.h"
#include "BitFarers/Characters/Player/PlayerCharacter.h"

AClientItemPickup::AClientItemPickup()
{
	bReplicates = false;
}

void AClientItemPickup::SetItemData()
{
	HandleIdleTime();
	
	if (ItemData.ItemCategory == EItemCategory::Ammunition)
	{
		const FString ItemName = ItemData.ItemName.ToString();

		ClassifyAmmoType(ItemName);
	}

	if (ItemData.ItemMeshes.Num() < 0)
	{
		PRINT("MISSING ITEM MESH ON DATAROW");
		return;
	}

	USkeletalMesh* RandomMesh = ItemData.ItemMeshes[FMath::RandRange(0, ItemData.ItemMeshes.Num() - 1)];
	this->PickupMesh->SetSkeletalMesh(RandomMesh);

	InitializeMaterials();
}


void AClientItemPickup::InitializePickup(FItemData InItemData, int32 InItemCount, float InIdleTime)
{
	IdleTime = InIdleTime;

	this->ItemData = InItemData;
	this->ItemCount = InItemCount;
	
	SetItemData();
}

bool AClientItemPickup::Prerequisites()
{
	return true;
}

void AClientItemPickup::ExecutePickup()
{
	if (ItemData.ItemCategory == EItemCategory::Ammunition)
	{
		InterpolationTarget->AmmoComponent->AddAmmo(AmmoType, ItemCount);
	}
	else
	{
		// Inventory pickup.
		PRINT("x0318 Todo: Implement");
	}
}