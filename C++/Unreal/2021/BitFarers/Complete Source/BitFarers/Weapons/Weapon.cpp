// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"


#include "Ammo/AmmoComponent.h"
#include "BitFarers/Macros.h"
#include "BitFarers/Characters/Player/WeaponComponent.h"
#include "Components/ArrowComponent.h"

AWeapon::AWeapon(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	INI_COMP_ATTACH(Transform, RootComponent, USceneComponent, "Transform");
	INI_COMP_ATTACH(Parent, Transform, USceneComponent, "Parent");
	INI_COMP_ATTACH(WeaponSK, Parent, USkeletalMeshComponent, "WeaponSK");

	INI_COMP_ATTACH(ShootDirection, WeaponSK, USceneComponent, "ShootDirection");
	
	INI_COMP_ATTACH(ShootDirectionMarker, ShootDirection, UArrowComponent, "ShootDirectionMarker");
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = GetStats().CalculateDynamicMaxAmmoClip();

	if (WeaponSK->SkeletalMesh)
	{
		if (WeaponSK->SkeletalMesh->FindSocket("ShootDirection"))
		{
			ShootDirection->AttachToComponent(WeaponSK,	FAttachmentTransformRules::SnapToTargetIncludingScale, FName("ShootDirection"));
		}
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::InputShoot()
{
	
	// Tell the player 'we want to shoot!'
	PreShootingDelegate.Broadcast();
	RequestShoot();
}

void AWeapon::StopInputShoot()
{
	if (ShootHandler.IsValid())
	{
		// Tell the player 'we want to stop shooting!' - if we are actually shooting
		PostShootingDelegate.Broadcast();
	}

	auto& WorldTimeManager = GetWorldTimerManager();

	WorldTimeManager.ClearTimer(ShootHandler);
	ShootHandler.Invalidate();

	if (!InternalCooldownHandler.IsValid())
	{
		WorldTimeManager.SetTimer(InternalCooldownHandler, this, &AWeapon::StopInternalCooldown,
		                          GetStats().CalculateDynamicRateOfFire(), false, -1);
	}
}

void AWeapon::StopInternalCooldown()
{
	bInternalCooldown = false;
	InternalCooldownHandler.Invalidate();
}

// check if the player is stunned or reloading, so 'physically-able'.
bool AWeapon::PhysicallyCanShoot()
{
	// TODO: If player 'stunned' return false
	// TODO: If player 'reloading' return false

	return true;
}

void AWeapon::RequestShoot()
{
	if (PhysicallyCanShoot())
	{
		StartShooting();
	}
}

void AWeapon::SwitchFireMode()
{

	if (WeaponFireModeIndex + 1 > Stats.Num() - 1)
	{
		WeaponFireModeIndex = 0;
	} else
	{
		WeaponFireModeIndex++;
	}
	
	auto& WorldTimeManager = GetWorldTimerManager();

	if (ShootHandler.IsValid())
	{
		if (WorldTimeManager.IsTimerActive(ShootHandler))
		{
			UpdateShootHandlerTickRate();
		}
	}

	ShootModeChanged.Broadcast();
}

void AWeapon::UpdateShootHandlerTickRate()
{
	
	if (!bInternalCooldown)
	{
		Shoot();
	}

	if (GetStats().FiringType == EFiringType::Single)
	{
		StopInputShoot();
		return;
	}

	auto& WorldTimeManager = GetWorldTimerManager();

	WorldTimeManager.SetTimer(
		ShootHandler,
		this,
		&AWeapon::Shoot,
		GetStats().CalculateDynamicRateOfFire(),
		true,
		WorldTimeManager.GetTimerRemaining(InternalCooldownHandler));
}

void AWeapon::StartShooting()
{
	// Tell the player 'we have started trying to shoot!'
	StartShootingDelegate.Broadcast();

	// Start the ShootHandler delegate
	UpdateShootHandlerTickRate();
}

// Peek is used to see what's in the clip before shooting rather than during.
bool AWeapon::HasAmmo_Implementation(const bool bIsPeek)
{
	if (GetStats().AmmoType == EAmmoType::Energy)
	{
		auto* AmmoComponent = IAmmoInterface::Execute_RetrieveAmmoComponent(CharacterRef);
		int32 AmmoRemaining;

		// If we are peeking, we want to in general get only primary stats, no alt fire stats
		if (AmmoComponent->HasAmmo_Remaining(EAmmoType::Energy, GetStats().CalculateDynamicAmmoUsage(false), AmmoRemaining))
		{
			if (AmmoRemaining >= GetStats().CalculateDynamicAmmoUsage(false))
			{
				return true;
			}
		}

		if (!bIsPeek)
		{
			NoAmmoDelegate.Broadcast();
		}
		
		return false;
	}

	if (CurrentAmmo < GetStats().CalculateDynamicAmmoUsage(false))
	{
		if (!bIsPeek)
		{
			NoAmmoDelegate.Broadcast();

			if (GetStats().bAutoReload)
			{
				Reload();
			}
		}

		return false;
	}

	return true;
}

bool AWeapon::ShootPrerequisites_Implementation()
{
	return HasAmmo();
}

void AWeapon::Shoot()
{
	if (ShootPrerequisites())
	{
		for (int32 i = 0; i < (GetStats().FiringMode == EFiringMode::Burst ? GetStats().CalculateDynamicBurstCount() : 1); i++)
		{
			bInternalCooldown = true;

			if (GetStats().AmmoType != EAmmoType::Energy)
			{
				if (CurrentAmmo < GetStats().CalculateDynamicAmmoUsage(false))
				{
					break;
				}

				CurrentAmmo -= GetStats().CalculateDynamicAmmoUsage(false);
			} else
			{
				auto* AmmoComponent = IAmmoInterface::Execute_RetrieveAmmoComponent(CharacterRef);
				AmmoComponent->UseAmmo(EAmmoType::Energy, GetStats().CalculateDynamicAmmoUsage(false));
				
			}

			// Tell the player 'we have shot!'
			OnShootingDelegate.Broadcast(WeaponFireModeIndex);
		}
	} else
	{
		// We're stopping with shooting
		PostShootingDelegate.Broadcast();
	}
}

void AWeapon::Reload()
{
	if (ShootHandler.IsValid())
	{
		StopInputShoot();
	}

	// Return if already reloading.
	if (bIsReloading)
		return;

	// Energy weapons don't reload, so skip;
	if (GetStats().AmmoType == EAmmoType::Energy)
		return;

	StartReloadDelegate.Broadcast();

	if (CharacterRef)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(CharacterRef);
		if (PlayerCharacter)
		{
			int32 Remaining = 0;
			const int32 ToRetrieve = FMath::Abs(CurrentAmmo - GetStats().CalculateDynamicMaxAmmoClip());

			UAmmoComponent* AmmoComponent = PlayerCharacter->AmmoComponent;

			if (AmmoComponent->HasAmmo_Remaining(GetStats().AmmoType, ToRetrieve, Remaining))
			{
				PlayerCharacter->WeaponComponent->IsReloading();
				PlayerCharacter->WeaponComponent->Reload_Server(true);

				bIsReloading = true;

				AmmoComponent->UseAmmo(GetStats().AmmoType, Remaining);

				FTimerDelegate TimerDel;
				FTimerHandle TimerHandle;

				TimerDel.BindUFunction(this, FName("DoReload"), Remaining);
				GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, GetStats().CalculateDynamicReloadSpeed(), false);
			}
		}
		else
		{
			PRINT("Unimplemented possible non-players reloading?");
		}
	}
}

void AWeapon::DoReload(int32 Remaining)
{
	CurrentAmmo += Remaining;

	if (CharacterRef)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(CharacterRef);
		if (PlayerCharacter)
		{
			PlayerCharacter->WeaponComponent->Reload_Server(false);
		}
	}

	PostReloadDelegate.Broadcast();

	bIsReloading = false;
}

FWeaponStats AWeapon::GetStats()
{
	if (WeaponFireModeIndex >= Stats.Num() )
	{
		PRINT("this is bad!");
		check(nullptr);
	}
	
	return Stats[WeaponFireModeIndex];
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, bIsReloading);
	DOREPLIFETIME(AWeapon, CharacterRef);
}
