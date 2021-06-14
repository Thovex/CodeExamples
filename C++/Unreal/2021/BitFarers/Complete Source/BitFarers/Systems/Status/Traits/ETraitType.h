#pragma once

#include "ETraitType.generated.h"

UENUM(BlueprintType)
enum class ETraitType: uint8
{	
	AllDamage								UMETA(DisplayName = "AllDamage"),
	RateOfFire								UMETA(DisplayName = "RateOfFire"),
	CriticalHitChance						UMETA(DisplayName = "CriticalHitChance"),
	CriticalHitDamage						UMETA(DisplayName = "CriticalHitDamage"),

	MovementSpeed							UMETA(DisplayName = "MovementSpeed"),
	
	MaximumHealth							UMETA(DisplayName = "MaximumHealth"),
	MaximumShield							UMETA(DisplayName = "MaximumShield"),
	
	HealthRegen								UMETA(DisplayName = "HealthRegen"),
	ShieldRegen								UMETA(DisplayName = "ShieldRegen"),
	
	LifeSteal								UMETA(DisplayName = "LifeSteal"),

	ReloadSpeed								UMETA(DisplayName = "ReloadSpeed"),
	Accuracy								UMETA(DisplayName = "Accuracy"),

	DodgeChancePercentage					UMETA(DisplayName = "DodgeChancePercentage"),

	PhysicalResistance						UMETA(DisplayName = "PhysicalResistance"),
	FireResistance							UMETA(DisplayName = "FireResistance"),
	IceResistance							UMETA(DisplayName = "IceResistance"),
	EnergyResistance						UMETA(DisplayName = "EnergyResistance"),
	PoisonResistance						UMETA(DisplayName = "PoisonResistance"),
	ExplosionResistance						UMETA(DisplayName = "ExplosionResistance"),
	RootResistance							UMETA(DisplayName = "RootResistance"),
	SlowResistance							UMETA(DisplayName = "SlowResistance"),

	MoneyGain								UMETA(DisplayName = "MoneyGain"),
	ExperienceGain							UMETA(DisplayName = "ExperienceGain"),

	CooldownReduction						UMETA(DisplayName = "CooldownReduction"),
	DodgeCount								UMETA(DisplayName = "DodgeCount"),
    JumpCount								UMETA(DisplayName = "JumpCount"),
	
	MaxNormalAmmoCapacity					UMETA(DisplayName = "Max Normal Ammo Capacity"),
	MaxPenetrationAmmoCapacity				UMETA(DisplayName = "Max Penetration Ammo Capacity"),
	MaxEnergyAmmoCapacity					UMETA(DisplayName = "Max Energy Ammo Capacity"),
	MaxRadioactiveAmmoCapacity				UMETA(DisplayName = "Max Radioactive Ammo Capacity"),
	MaxExplosiveAmmoCapacity				UMETA(DisplayName = "Max Explosive Ammo Capacity"),
	EnergyRegeneration						UMETA(DisplayName = "Energy Regeneration"),
	
	UtilityCount							UMETA(DisplayName = "Utility Count"),

};
