// Copyright Baer and Hoggo Games


#include "ClientFoodPickup.h"


#include "BitFarers/Characters/Player/PlayerCharacter.h"
#include "BitFarers/Systems/Status/StatusComponent.h"

void AClientFoodPickup::BeginPlay()
{
	Super::BeginPlay();

	bPickupAble = true;
}

bool AClientFoodPickup::Prerequisites()
{
	if (InterpolationTarget)
	{
		UStatusComponent* StatusComponent = InterpolationTarget->StatusComponent;

		UTrait* MaxHealth = StatusComponent->GetTraitPointer(ETraitType::MaximumHealth);

		if (MaxHealth)
		{
			if (StatusComponent->CurrentHealth < MaxHealth->GetModified())
			{
				TargetComponent = StatusComponent;
				return true;
			}
		}
	}

	return false;
}

void AClientFoodPickup::ExecutePickup()
{
	if (TargetComponent)
	{
		TargetComponent->Heal_CurrentHealth_Server(ItemCount);
	}
}
