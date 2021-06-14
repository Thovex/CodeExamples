// Copyright Baer and Hoggo Games


#include "ClientAmmoPickup.h"



#include "BitFarers/Macros.h"
#include "BitFarers/Characters/Player/PlayerCharacter.h"
#include "BitFarers/Weapons/Ammo/AmmoComponent.h"

void AClientAmmoPickup::BeginPlay()
{
	Super::BeginPlay();

	bPickupAble = true;
}


bool AClientAmmoPickup::Prerequisites()
{
	if (InterpolationTarget)
	{
		UAmmoComponent* AmmoComponent = InterpolationTarget->AmmoComponent;

		if (!AmmoComponent->IsAmmoFull(AmmoType))
		{
			TargetComponent = AmmoComponent;
			return true;
		}
	}
	return false;
}

void AClientAmmoPickup::ExecutePickup()
{
	if (TargetComponent)
	{
		TargetComponent->AddAmmo(AmmoType, ItemCount);
	}
}

void AClientAmmoPickup::InitializePickup(FItemData InItemData, int32 InItemCount, float InIdleTime)
{
	Super::InitializePickup(InItemData, InItemCount, InIdleTime);

	AmmoType = static_cast<EAmmoType>(ItemData.DataInt);

}

