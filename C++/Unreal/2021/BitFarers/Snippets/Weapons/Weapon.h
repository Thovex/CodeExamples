// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



#include "EWeapon.h"
#include "WeaponStats.h"
#include "BitFarers/Characters/Player/PlayerCharacter.h"
#include "Engine/DataTable.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPreShooting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartShooting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShooting, int32, FireMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPostShooting);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShootModeChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoAmmo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartReload);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPostReload);


UCLASS(BlueprintType)
class BITFARERS_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon(const FObjectInitializer& ObjectInitializer);

	/* Required Components */

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Parent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* WeaponSK;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* ShootDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* ShootDirectionMarker;

	/* Delegates */

	UPROPERTY(BlueprintAssignable)
	FPreShooting PreShootingDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FStartShooting StartShootingDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnShooting OnShootingDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FPostShooting PostShootingDelegate;

	UPROPERTY(BlueprintAssignable)
	FNoAmmo NoAmmoDelegate;

	UPROPERTY(BlueprintAssignable)
	FShootModeChange ShootModeChanged;

	UPROPERTY(BlueprintAssignable)
	FStartReload StartReloadDelegate;

	UPROPERTY(BlueprintAssignable)
	FPostReload PostReloadDelegate;

	/* Properties */

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWeaponStats> Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeapon Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* HitEmitters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* WeaponIcon;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool bIsReloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponFireModeIndex = 0;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
    ACharacter* CharacterRef;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable)
	void InputShoot();

	UFUNCTION(BlueprintCallable)
	void StopInputShoot();
	void StopInternalCooldown();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ShootPrerequisites();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool HasAmmo(bool bIsPeek = false);

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION()
	void DoReload(int32 Remaining);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWeaponStats GetStats();

	FTimerHandle ShootHandler;

	UFUNCTION(BlueprintCallable)
	void SwitchFireMode();

private:
	FTimerHandle InternalCooldownHandler;
	
	bool bInternalCooldown = false;

	void RequestShoot();
	void UpdateShootHandlerTickRate();
	void StartShooting();
	void Shoot();
	

	bool PhysicallyCanShoot();
};
