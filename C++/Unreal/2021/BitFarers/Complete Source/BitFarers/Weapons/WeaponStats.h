// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "EElementalType.h"
#include "EFiringMode.h"
#include "EFiringStyle.h"
#include "EFiringType.h"
#include "Ammo/EAmmoType.h"
#include "BitFarers/Systems/Status/ItemAttributes.h"
#include "Engine/DataTable.h"
#include "WeaponStats.generated.h"

class AWeapon;

USTRUCT(BlueprintType)
struct FWeaponStats : public FTableRowBase
{
	GENERATED_BODY()

	/*
	 *	W	A	R	N	I	N	G 
	 *
	 *	If you make a new variable in the weapon struct:
	 *	Modify it in the DropLibrary.h struct too! Add it to the constructor.
	 *  
	 *	W	A	R	N	I	N	G 
	 */


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponName = "Unnamed";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType = EAmmoType::Light;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EElementalType ElementalType = EElementalType::Physical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFiringStyle FiringStyle = EFiringStyle::HitScan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFiringType FiringType = EFiringType::Automatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFiringMode FiringMode = EFiringMode::Single;

	// Does weapon auto reload on no ammo?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoReload = true;
	
	// What the player gains by equipping this item in Traits.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemAttributes Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon> WeaponClass;

	float CalculateDynamicDamage() const;
	float CalculateDynamicHitRange() const;
	float CalculateDynamicRateOfFire() const;
	float CalculateDynamicReloadSpeed() const;
	float CalculateDynamicRange() const;
	float CalculateDynamicHitSpread() const;
	int32 CalculateDynamicAmmoUsage(const bool bBurstCheck) const;
	int32 CalculateDynamicMaxAmmoClip() const;
	int32 CalculateDynamicBurstCount() const;
	int32 CalculateDynamicProjectileCount() const;

	void SetBaseDamage(const float InValue);
	void SetBaseHitSpread(const float InValue);
	void SetBaseRange(const float InValue);
	void SetBaseReloadSpeed(const float InValue);
	void SetBaseRateOfFire(const float InValue);
	void SetBaseAmmoUsage(const float InValue);
	void SetMaxAmmoClip(const float InValue);
	void SetBaseBurstCount(const int32 InValue);
	void SetBaseProjectileCount(const int32 InValue);

	float GetBaseDamage() const;
	float GetBaseHitSpread() const;
	float GetBaseRange() const;
	float GetBaseReloadSpeed() const;
	float GetBaseRateOfFire() const;
	float GetBaseAmmoUsage() const;
	float GetMaxAmmoClip() const;
	int32 GetBaseBurstCount() const;
	int32 GetBaseProjectileCount() const;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage = 0;

	// Otherwise known as 'Accuracy' - except inverted.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseHitSpread = 0;

	// Raycast Range
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseRange = 2000;

	// in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseReloadSpeed = 1;

	// in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseRateOfFire = 0.5;

	// Will be rounded to an int on non-energy.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseAmmoUsage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseProjectileCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseBurstCount = 3;

	// Total clip size
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAmmoClip = 1;


};

UCLASS()
class BITFARERS_API UWeaponStatsFunctionLibrary: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		
	/*Getters*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FWeaponStats|Methods")	
        static float GetDynamicDamage(UPARAM(ref) FWeaponStats& Stats) { return Stats.CalculateDynamicDamage(); }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FWeaponStats|Methods")	
        static float GetDynamicRateOfFire(UPARAM(ref) FWeaponStats& Stats) { return Stats.CalculateDynamicRateOfFire(); }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FWeaponStats|Methods")	
        static float GetDynamicReloadSpeed(UPARAM(ref) FWeaponStats& Stats) { return Stats.CalculateDynamicReloadSpeed(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FWeaponStats|Methods")	
		static float GetDynamicRange(UPARAM(ref) FWeaponStats& Stats) { return Stats.CalculateDynamicRange(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FWeaponStats|Methods")	
		static float GetDynamicHitSpread(UPARAM(ref) FWeaponStats& Stats) { return Stats.CalculateDynamicHitSpread(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FWeaponStats|Methods")	
		static int32 GetDynamicAmmoUsage(UPARAM(ref) FWeaponStats& Stats, const bool bBurstCheck) { return Stats.CalculateDynamicAmmoUsage(bBurstCheck); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FWeaponStats|Methods")	
		static int32 GetDynamicMaxAmmoClip(UPARAM(ref) FWeaponStats& Stats) { return Stats.CalculateDynamicMaxAmmoClip(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FWeaponStats|Methods")	
        static int32 GetDynamicProjectileCount(UPARAM(ref) FWeaponStats& Stats) { return Stats.CalculateDynamicProjectileCount(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FWeaponStats|Methods")	
		static int32 GetDynamicBurstCount(UPARAM(ref) FWeaponStats& Stats) { return Stats.CalculateDynamicBurstCount(); }


	
};