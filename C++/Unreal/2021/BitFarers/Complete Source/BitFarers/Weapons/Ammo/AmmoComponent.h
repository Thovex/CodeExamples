// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Ammo.h"
#include "EAmmoType.h"
#include "Components/ActorComponent.h"
#include "AmmoComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAmmoChange, EAmmoType, Type, int32, Current, int32, Max);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BITFARERS_API UAmmoComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAmmoComponent();

#pragma region Delegates
	
#pragma region Light
	UPROPERTY(BlueprintAssignable)
	FAmmoChange LightGainDelegate;

	UPROPERTY(BlueprintAssignable)
	FAmmoChange LightMaxChangeDelegate;

	UPROPERTY(BlueprintAssignable)
	FAmmoChange LightLossDelegate;
#pragma endregion
#pragma region Medium

	UPROPERTY(BlueprintAssignable)
	FAmmoChange MediumGainDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FAmmoChange MediumMaxChangeDelegate;

	UPROPERTY(BlueprintAssignable)
	FAmmoChange MediumLossDelegate;
#pragma endregion
#pragma region Energy

	UPROPERTY(BlueprintAssignable)
	FAmmoChange EnergyGainDelegate;

	UPROPERTY(BlueprintAssignable)
	FAmmoChange EnergyMaxChangeDelegate;

	UPROPERTY(BlueprintAssignable)
	FAmmoChange EnergyLossDelegate;
#pragma endregion
#pragma region Heavy

	UPROPERTY(BlueprintAssignable)
	FAmmoChange HeavyGainDelegate;

	UPROPERTY(BlueprintAssignable)
	FAmmoChange HeavyMaxChangeDelegate;

	UPROPERTY(BlueprintAssignable)
	FAmmoChange HeavyLossDelegate;
#pragma endregion

	
#pragma endregion 
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetAmmo_Current(const EAmmoType AmmoType, const int32 NewCount );

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetAmmo_Max(const EAmmoType AmmoType, const int32 NewCount );

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void AddAmmo(const EAmmoType AmmoType, const int32 Amount);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void UseAmmo(const EAmmoType AmmoType, const int32 Amount);
	
	UFUNCTION(BlueprintCallable)
	bool HasAmmo_Remaining(const EAmmoType AmmoType, int32 Count, int32& RemainingCount);
	bool IsAmmoFull(EAmmoType AmmoType);

	UFUNCTION()
	void OnRep_OnAmmoChanged(TArray<FAmmo> PreviousAmmo);

	UFUNCTION(BlueprintCallable)
	void CallAllDelegates();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_OnAmmoChanged)
	TArray<FAmmo> Ammo;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetTotalAmmo_Remaining(EAmmoType AmmoType);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetTotalAmmo_Max(EAmmoType AmmoType);


protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

private:
	
	TMap<EAmmoType, int32> AmmoTypeIndex;

	void CallCurrentChangeDelegate(const EAmmoType Type, const bool Gain, const int32 Current, const int32 Max) const;
	void CallMaxChangeDelegate(const EAmmoType Type, const int32 Current, const int32 Max) const;
	bool HasAmmo(EAmmoType AmmoType, int32 Count);


};
