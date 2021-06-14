// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoComponent.h"


#include "BitFarers/Macros.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UAmmoComponent::UAmmoComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Ammo.Empty();

	for (int32 i = 0; i < 5; i++)
	{
		Ammo.Add(FAmmo(static_cast<EAmmoType>(i), 0, 0));
		AmmoTypeIndex.Add(static_cast<EAmmoType>(i), i);
	}

	SetIsReplicatedByDefault(true);
}

void UAmmoComponent::SetAmmo_Current_Implementation(const EAmmoType AmmoType, const int32 NewCount)
{
	const auto PreviousAmmo = Ammo;
	Ammo[AmmoTypeIndex[AmmoType]].Current = NewCount;
	OnRep_OnAmmoChanged(PreviousAmmo);
}

void UAmmoComponent::SetAmmo_Max_Implementation(const EAmmoType AmmoType, const int32 NewCount)
{
	const auto PreviousAmmo = Ammo;
	Ammo[AmmoTypeIndex[AmmoType]].Max = NewCount;
	OnRep_OnAmmoChanged(PreviousAmmo);
}

void UAmmoComponent::UseAmmo_Implementation(const EAmmoType AmmoType, const int32 Count)
{
	AddAmmo(AmmoType, -Count);
}

void UAmmoComponent::AddAmmo_Implementation(const EAmmoType AmmoType, const int32 Amount)
{
	const auto PreviousAmmo = Ammo;
	FAmmo& ThisAmmo = Ammo[AmmoTypeIndex[AmmoType]];
	
	if (Amount == 0) return;

	if (Amount > 0)
	{
		if (ThisAmmo.Current + Amount >= ThisAmmo.Max)
		{
			ThisAmmo.Current = ThisAmmo.Max;
		} else
		{
			ThisAmmo.Current += Amount;
		}

	} else if (Amount < 0)
	{
		if (ThisAmmo.Current + Amount <= 0)
		{
			ThisAmmo.Current = 0;
		} else
		{
			ThisAmmo.Current += Amount;
		}
	}
	
	OnRep_OnAmmoChanged(PreviousAmmo);
}




void UAmmoComponent::CallMaxChangeDelegate(const EAmmoType Type, const int32 Current, const int32 Max) const
{
	switch (Type)
	{
	case EAmmoType::Light:
		{
			LightMaxChangeDelegate.Broadcast(Type, Current, Max);
		}
	case EAmmoType::Medium:
		{
			MediumMaxChangeDelegate.Broadcast(Type, Current, Max);
		}
	case EAmmoType::Energy:
		{
			EnergyMaxChangeDelegate.Broadcast(Type, Current, Max);
		}
	case EAmmoType::Heavy:
		{
			HeavyMaxChangeDelegate.Broadcast(Type, Current, Max);
		}

	}
}

void UAmmoComponent::CallCurrentChangeDelegate(const EAmmoType Type, const bool Gain, const int32 Current, const int32 Max) const
{
	if (Gain)
	{
		switch (Type)
		{
		case EAmmoType::Light:
			{
				LightGainDelegate.Broadcast(Type, Current, Max);
			}
		case EAmmoType::Medium:
			{
				MediumGainDelegate.Broadcast(Type, Current, Max);
			}
		case EAmmoType::Energy:
			{
				EnergyGainDelegate.Broadcast(Type, Current, Max);
			}
		case EAmmoType::Heavy:
			{
				HeavyGainDelegate.Broadcast(Type, Current, Max);
			}
		}
	} else
	{
		switch (Type)
		{
		case EAmmoType::Light:
			{
				LightLossDelegate.Broadcast(Type, Current, Max);
			}
		case EAmmoType::Medium:
			{
				MediumLossDelegate.Broadcast(Type, Current, Max);
			}
		case EAmmoType::Energy:
			{
				EnergyLossDelegate.Broadcast(Type, Current, Max);
			}
		case EAmmoType::Heavy:
			{
				HeavyLossDelegate.Broadcast(Type, Current, Max);
			}
		}
	}
}

int32 UAmmoComponent::GetTotalAmmo_Remaining(const EAmmoType AmmoType)
{
	return Ammo[AmmoTypeIndex[AmmoType]].Current;
}

int32 UAmmoComponent::GetTotalAmmo_Max(const EAmmoType AmmoType)
{
	return Ammo[AmmoTypeIndex[AmmoType]].Max;

}

bool UAmmoComponent::HasAmmo(const EAmmoType AmmoType, const int32 Count)
{
	return Ammo[AmmoTypeIndex[AmmoType]].Current >= Count;
}

bool UAmmoComponent::HasAmmo_Remaining(const EAmmoType AmmoType, const int32 Count, int32& RemainingCount)
{
	if (HasAmmo(AmmoType, Count))
	{
		RemainingCount = Count;
		return true;
	}

	if (Ammo[AmmoTypeIndex[AmmoType]].Current > 0)
	{
		RemainingCount = Ammo[AmmoTypeIndex[AmmoType]].Current;
		return true;
	}

	RemainingCount = 0;
	return false;
}

bool UAmmoComponent::IsAmmoFull(const EAmmoType AmmoType)
{
	return Ammo[AmmoTypeIndex[AmmoType]].Current == Ammo[AmmoTypeIndex[AmmoType]].Max;
}

void UAmmoComponent::OnRep_OnAmmoChanged(TArray<FAmmo> PreviousAmmo)
{
	for (auto& Now : Ammo)
	{
		for (auto& Before : PreviousAmmo)
		{
			if (Now.AmmoType == Before.AmmoType)
			{
				if (Now.Current != Before.Current)
				{	
					if (Now.Current > Before.Current)
					{
						CallCurrentChangeDelegate(Now.AmmoType, true, Now.Current, Now.Max);
					} else if (Now.Current < Before.Current)
					{
						CallCurrentChangeDelegate(Now.AmmoType, false, Now.Current, Now.Max);
					}
				}

				if (Now.Max != Before.Max)
				{
					CallMaxChangeDelegate(Now.AmmoType, Now.Current, Now.Max);
				}
			}
		}
	}	
}

void UAmmoComponent::CallAllDelegates()
{
	
	for (const auto& AnAmmo : Ammo)
	{
		switch (AnAmmo.AmmoType)
		{
		case EAmmoType::Light: 
			{
				LightGainDelegate.Broadcast(AnAmmo.AmmoType, AnAmmo.Current,AnAmmo.Max );
			} break;
		case EAmmoType::Medium: 
			{
				MediumGainDelegate.Broadcast(AnAmmo.AmmoType, AnAmmo.Current,AnAmmo.Max );
			} break;
		case EAmmoType::Energy:
			{
				EnergyGainDelegate.Broadcast(AnAmmo.AmmoType, AnAmmo.Current,AnAmmo.Max );
			} break;
		case EAmmoType::Heavy: 
			{
				HeavyGainDelegate.Broadcast(AnAmmo.AmmoType, AnAmmo.Current,AnAmmo.Max );
			} break;
		}
	}
}

void UAmmoComponent::BeginPlay()
{
	Super::BeginPlay();
	
	SetAmmo_Max(EAmmoType::Light, 256);
	SetAmmo_Current(EAmmoType::Light, 256);

	SetAmmo_Max(EAmmoType:: Medium, 128);
	SetAmmo_Current(EAmmoType::Medium, FMath::RoundToInt(128/2));

	SetAmmo_Max(EAmmoType::Heavy, 64);
	SetAmmo_Current(EAmmoType::Heavy, FMath::RoundToInt(64/4));

	SetAmmo_Max(EAmmoType::Energy, 100);
	SetAmmo_Current(EAmmoType::Energy, 100);

}

void UAmmoComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAmmoComponent, Ammo);
}

