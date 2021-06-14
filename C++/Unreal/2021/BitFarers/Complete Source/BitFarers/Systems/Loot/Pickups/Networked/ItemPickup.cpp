// Copyright Baer and Hoggo Games


#include "ItemPickup.h"


#include "BitFarers/Macros.h"
#include "BitFarers/Characters/Player/PlayerCharacter.h"
#include "Net/UnrealNetwork.h"

AItemPickup::AItemPickup()
{
	bReplicates = true;
}

void AItemPickup::OnRep_SetItemData()
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


void AItemPickup::InitializePickup(FItemData InItemData, int32 InItemCount, float InIdleTime)
{
	IdleTime = InIdleTime;
	InitializePickup_Response(InItemData, InItemCount);
}

void AItemPickup::InitializePickup_Response_Implementation(FItemData InItemData, int32 InItemCount)
{
	this->ItemData = InItemData;
	this->ItemCount = InItemCount;

	OnRep_SetItemData();
}

bool AItemPickup::Prerequisites()
{
	return true;
}

void AItemPickup::ExecutePickup()
{
	PRINT("x0317 Todo: Implement");
}

void AItemPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemPickup, ItemData);
	DOREPLIFETIME(AItemPickup, ItemCount);
}
