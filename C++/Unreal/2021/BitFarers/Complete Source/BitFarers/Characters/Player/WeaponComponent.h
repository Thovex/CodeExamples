// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"


#include "BitFarers/Systems/Loot/Pickups/Networked/WeaponPickup.h"
#include "BitFarers/Weapons/WeaponStats.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class AWeapon;
class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSwapWeapon, AWeapon*, NewWeapon, int32, Index);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BITFARERS_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();
	void AcquireOwner();

	virtual void BeginPlay() override;

	/*
	 *  CORE: Shooting Weapons
	*/

	void OnFire_Start_Request() const;
	void OnFire_End_Request() const;

	/*
	 *	CORE: Swap weapon functionality index
	 */
	
	void OnSwapWeaponMode_Request() const;

	/* 
	*   CORE: Swapping Weapons
	*/

	// Functions

    void SwapWeapon_Request(int32 WeaponIndex);
	
	UFUNCTION(Server, Reliable, Category = Weapon)
    void SwapWeapon_Response(int32 WeaponIndex);
    
	UPROPERTY(BlueprintAssignable)
	FSwapWeapon SwapWeaponDelegate;

	/* 
	*   CORE: Equipping Weapon
	*/

	// Properties
	
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_WeaponChanged, BlueprintReadWrite, Category = Weapon)
	AWeapon* CurrentEquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int32 CurrentEquippedWeaponIndex = 0;

	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_WeaponCollectionChanged, BlueprintReadWrite, Category = Weapon)
	TArray<AWeapon*> CurrentWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int32 MaxCurrentWeapons = 3;

	bool CheckWeaponSlotAvailability(int32& EmptyIndex);

	// Functions

	UFUNCTION(BlueprintCallable, Category = Weapon)
    void EquipWeapon_Request(const TArray<FWeaponStats>& WeaponStats);
	
	UFUNCTION(Server, Reliable, Category = Weapon)
    void EquipWeapon_Response(const TArray<FWeaponStats>& WeaponStats);

	// RepNotify
	
	UFUNCTION()
    void OnRep_WeaponChanged();
	
	UFUNCTION()
    void OnRep_WeaponCollectionChanged() const;
	
	/* 
	*   CORE: Reloading
	*/

	// Properties
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	bool bIsReloading = false;
	
	// Functions
	
	UFUNCTION(BlueprintCallable, Category = Weapon)
    void Reload_Request() const;
	void IsReloading() const;

	UFUNCTION(Server, Unreliable, Category = Weapon)
    void Reload_Server(bool SetReload);

	
	/* 
	*   CORE: Dropping Weapon
	*/

	// Properties
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeaponPickup> DropItemClass;

	// Functions
	
	UFUNCTION(BlueprintCallable, Category = Weapon)
    void DropWeapon_Request();

	UFUNCTION(Server, Reliable, Category = Weapon)
    void DropWeapon_Response();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY()
	APlayerCharacter* Character;
	
};
