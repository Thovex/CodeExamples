#include "WeaponStats.h"

float FWeaponStats::CalculateDynamicDamage() const
{	
	return BaseDamage;
}

float FWeaponStats::CalculateDynamicHitRange() const
{
	return BaseRange;
}

float FWeaponStats::CalculateDynamicRateOfFire() const
{
	if (Attributes.TraitCollection.Traits.Contains(ETraitType::RateOfFire))
	{
		auto& RateOfFire = Attributes.TraitCollection.Traits[ETraitType::RateOfFire];
		return BaseRateOfFire * RateOfFire;
	}

	return BaseRateOfFire;
}

float FWeaponStats::CalculateDynamicReloadSpeed() const
{
	if (Attributes.TraitCollection.Traits.Contains(ETraitType::ReloadSpeed))
	{
		auto& ReloadSpeed = Attributes.TraitCollection.Traits[ETraitType::ReloadSpeed];
		return BaseReloadSpeed * ReloadSpeed;
	}
	
	return BaseReloadSpeed;
}

float FWeaponStats::CalculateDynamicRange() const
{
	// Maybe an attribute?
	
	return BaseRange;
}

float FWeaponStats::CalculateDynamicHitSpread() const
{
	return BaseHitSpread;
}

int32 FWeaponStats::CalculateDynamicAmmoUsage(const bool bBurstCheck) const
{
	// Maybe an attribute?

	if (bBurstCheck)
	{
		return BaseAmmoUsage * (FiringMode == EFiringMode::Burst ? CalculateDynamicBurstCount() : 1);
	}

	return BaseAmmoUsage;
}

int32 FWeaponStats::CalculateDynamicMaxAmmoClip() const
{
	return MaxAmmoClip;
}

int32 FWeaponStats::CalculateDynamicBurstCount() const
{
	return BaseBurstCount;
}

int32 FWeaponStats::CalculateDynamicProjectileCount() const
{
	return BaseProjectileCount;
}

void FWeaponStats::SetBaseDamage(const float InValue)
{
	BaseDamage = InValue;
}

void FWeaponStats::SetBaseHitSpread(const float InValue)
{
	BaseHitSpread = InValue;
}

void FWeaponStats::SetBaseRange(const float InValue)
{
	BaseRange = InValue;
}

void FWeaponStats::SetBaseReloadSpeed(const float InValue)
{
	BaseReloadSpeed = InValue;
}

void FWeaponStats::SetBaseRateOfFire(const float InValue)
{
	BaseRateOfFire = InValue;
}

void FWeaponStats::SetBaseAmmoUsage(const float InValue)
{
	BaseAmmoUsage = InValue;
}

void FWeaponStats::SetBaseProjectileCount(const int32 InValue)
{
	BaseProjectileCount = InValue;
}

void FWeaponStats::SetMaxAmmoClip(const float InValue)
{
	MaxAmmoClip = InValue;
}

void FWeaponStats::SetBaseBurstCount(const int32 InValue)
{
	BaseBurstCount = InValue;
}

float FWeaponStats::GetBaseDamage() const
{
	return BaseDamage;
}

float FWeaponStats::GetBaseHitSpread() const
{
	return BaseHitSpread;
}

float FWeaponStats::GetBaseRange() const
{
	return BaseRange;
}

float FWeaponStats::GetBaseReloadSpeed() const
{
	return BaseReloadSpeed;
}

float FWeaponStats::GetBaseRateOfFire() const
{
	return BaseRateOfFire;
}

float FWeaponStats::GetBaseAmmoUsage() const
{
	return BaseAmmoUsage;
}

float FWeaponStats::GetMaxAmmoClip() const
{
	return MaxAmmoClip;
}

int32 FWeaponStats::GetBaseBurstCount() const
{
	return BaseBurstCount;
}

int32 FWeaponStats::GetBaseProjectileCount() const
{
	return BaseProjectileCount;
}


