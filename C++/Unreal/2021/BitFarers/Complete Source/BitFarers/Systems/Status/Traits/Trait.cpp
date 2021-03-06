// Copyright Baer and Hoggo Games

#pragma once

#include "Trait.h"
#include "BitFarers/Macros.h"
#include "BitFarers/Systems/Status/StatusComponent.h"

UTrait::UTrait()
{
	// Do note. garbage collection does not occur
	AddToRoot();

	TraitModifiers.Empty();
}

void UTrait::Initialize(const ETraitType InType, const float InBase, const float InMin, const float InMax, UStatusComponent* InOuter)
{
	this->Type = InType;
	this->Base = InBase;
	this->Min = InMin;
	this->Max = InMax;
	this->Outer = InOuter;
}

float UTrait::GetModified()
{
	return CalculateModified();
}

bool UTrait::IsMaximumValue()
{
	return Max == GetModified();
}

bool UTrait::IsMinimumValue()
{
	return Min == GetModified();
}

int32 UTrait::GetModifierCount() const
{
	return TraitModifiers.Num();
}

FGuid UTrait::AddNewModifier(FTraitModifier Modifier)
{
	const float PreviousPower = GetModified();
	
	const FGuid GUID = FGuid::NewGuid();
	Modifier.GUID = GUID;

	if (Modifier.Duration > 0.0f)
	{
		if (Outer->GetWorld())
		{
			FTimerDelegate TimerDel;
			FTimerHandle TimerHandle;
		
			TimerDel.BindUFunction(this, FName("RemoveModifierByGUID"), Modifier.GUID);
			Outer->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, Modifier.Duration, false);
		} else
		{
			PRINT("Can't find world");
		}
	}

	TraitModifiers.Add(Modifier);
	
	OnModifierAdded.Broadcast(Modifier.Name, Modifier.Duration, Modifier.Power, Modifier.Type);
	OnChange.Broadcast(PreviousPower, GetModified());
	
	return GUID;
}

void UTrait::RemoveModifierByGUID(const FGuid Guid)
{
	const float PreviousPower = GetModified();

	int32 RemoveIdx = -1;
	
	for (int32 i = 0; i < TraitModifiers.Num(); i++)
	{
		if (TraitModifiers[i].GUID == Guid)
		{
			RemoveIdx = i;
			break;
		}
	}

	if (RemoveIdx != -1)
	{
		TraitModifiers.RemoveAt(RemoveIdx);
		OnChange.Broadcast(PreviousPower, GetModified());
	}
}


bool UTrait::IsSupportedForNetworking() const
{
	return true;
}

float UTrait::CalculateModified()
{
	float Modified = Base;

	// Todo: IsDirty marker and then skip all calculations if nothing changed.
	
	TArray<float> Additives;
	TArray<float> Multipliers;
	TArray<float> Dividers;

	for (const auto TraitModifier : TraitModifiers)
	{
		switch (TraitModifier.Type)
		{
		case EModifierType::Additive:
			{
				Additives.Add(TraitModifier.Power);
			}
			break;
		case EModifierType::Multiplier:
			{
				Multipliers.Add(TraitModifier.Power);
			}
			break;
		case EModifierType::Divider:
			{
				Dividers.Add(TraitModifier.Power);
			}
			break;
		case EModifierType::Override:
			{
				return TraitModifier.Power;
			}
		default:
			{
				PRINT("Trait Problem in CalculateModified");
			}
		}
	}

	for (const auto Additive : Additives)
	{
		Modified += Additive;
	}

	for (const auto Multiply : Multipliers)
	{
		Modified *= 1 + Multiply;
	}

	for (const auto Divide : Dividers)
	{
		if (Divide != 0)
		{
			Modified /= Divide;
		}
	}

	if (Modified < Min)
	{
		Modified = Min;
	}
	else if (Modified > Max)
	{
		Modified = Max;
	}

	return Modified;
}

float UTrait::GetBase() const
{
	return Base;
}

float UTrait::GetMin() const
{
	return Min;
}

float UTrait::GetMax() const
{
	return Max;
}
