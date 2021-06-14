// Copyright Baer and Hoggo Games


#include "FoodPickup.h"
#include "BitFarers/Systems/Status/StatusComponent.h"
#include "BitFarers/Characters/Player/PlayerCharacter.h"

void AFoodPickup::BeginPlay()
{
	Super::BeginPlay();

	bPickupAble = true;
}

bool AFoodPickup::Prerequisites()
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

void AFoodPickup::ExecutePickup()
{
	if (TargetComponent)
	{
		TargetComponent->Heal_CurrentHealth_Server(ItemCount);
	}
}
