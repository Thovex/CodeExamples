// Copyright Baer and Hoggo Games


#include "WeaponComponent.h"

#include "PlayerAnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "BitFarers/Utilities/Components/SoundCasterComponent.h"
#include "BitFarers/Macros.h"
#include "BitFarers/Systems/Loot/Pickups/Networked/WeaponPickup.h"
#include "BitFarers/Weapons/Weapon.h"


UWeaponComponent::UWeaponComponent()
{
	SetIsReplicatedByDefault(true);

	for (int32 i = 0; i < MaxCurrentWeapons; i++)
	{
		CurrentWeapons.Add(nullptr);
	}
}

void UWeaponComponent::AcquireOwner()
{
	AActor* Owner = GetOwner();

	if (Owner)
	{
		if (Cast<APlayerCharacter>(Owner))
		{
			Character = Cast<APlayerCharacter>(Owner);
		} else
		{
			PRINT("PlayerWeaponComponent on non-APlayerCharacter owner!");
		}
	}
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AcquireOwner();	
}

void UWeaponComponent::OnFire_Start_Request() const
{
	if (CurrentEquippedWeapon)
	{
		CurrentEquippedWeapon->InputShoot();
	}
}

void UWeaponComponent::OnFire_End_Request() const
{
	if (CurrentEquippedWeapon)
	{
		CurrentEquippedWeapon->StopInputShoot();
	}
}

void UWeaponComponent::OnSwapWeaponMode_Request() const
{
	if (CurrentEquippedWeapon)
	{
		CurrentEquippedWeapon->SwitchFireMode();

	}
}

void UWeaponComponent::Reload_Request() const
{
	if (CurrentEquippedWeapon)
	{
		if (!CurrentEquippedWeapon->bIsReloading)
		{
			this->CurrentEquippedWeapon->Reload();
		}
	}
}

void UWeaponComponent::IsReloading() const
{
	if (Character->IsA(APlayerCharacter::StaticClass()))
	{
		
		Cast<APlayerCharacter>(Character)->SoundCasterComponent->PlayOneShotSoundAttached_Name(true, "Reload",
         FMath::RandRange(0.9F, 1.1F), FMath::RandRange(0.9F, 1.1F));
	}
}

void UWeaponComponent::Reload_Server_Implementation(bool SetReload)
{
	bIsReloading = SetReload;
}

void UWeaponComponent::OnRep_WeaponChanged()
{
	if (!GetWorld()) return; 
	if (CurrentWeapons.Num() <= 0) return;

	for (auto& Weapon : CurrentWeapons)
	{
		if (Weapon)
		{
			if (Weapon == CurrentEquippedWeapon)
			{
				Weapon->SetActorTickEnabled(true);

			} else
			{
				Weapon->SetActorTickEnabled(false);

				if (Weapon->ShootHandler.IsValid())
				{
					Weapon->StopInputShoot();
				}
			}
		}
	}

	if (CurrentEquippedWeapon)
	{
		if (!Character)
		{
			AcquireOwner();
		}
		
		CurrentEquippedWeapon->CharacterRef = Character;

		const FAttachmentTransformRules Rules(
            EAttachmentRule::SnapToTarget,
            EAttachmentRule::SnapToTarget,
            EAttachmentRule::SnapToTarget,
            true
        );


		if (Character->GetFPS()->GetSocketByName(FName("GunSocket_"+CurrentEquippedWeapon->Stats[0].WeaponName.ToString())) == nullptr)
		{
			PRINT("NO SOCKET AVAILABLE FOR GUN: GunSocket_"+CurrentEquippedWeapon->Stats[0].WeaponName.ToString());
			return;
		}
		
		if (Character->IsLocallyControlled())
		{

			CurrentEquippedWeapon->AttachToComponent(
                Character->GetFPS(),
                Rules,
                FName("GunSocket_"+CurrentEquippedWeapon->Stats[0].WeaponName.ToString())
            );
		}
		else
		{
			CurrentEquippedWeapon->AttachToComponent(
                Character->GetTPS(),
                Rules,
                FName("GunSocket_"+CurrentEquippedWeapon->Stats[0].WeaponName.ToString())
            );
		}
	}

	SwapWeaponDelegate.Broadcast(CurrentEquippedWeapon, CurrentEquippedWeaponIndex);
}

void UWeaponComponent::OnRep_WeaponCollectionChanged() const
{
	if (!GetWorld()) return; 

	if (CurrentEquippedWeapon)
	{
		//	OnWeaponPickupEvent.Broadcast();
	}
}



void UWeaponComponent::SwapWeapon_Request(int32 WeaponIndex)
{
	if (bIsReloading) return;
	SwapWeapon_Response(WeaponIndex);
}

void UWeaponComponent::SwapWeapon_Response_Implementation(int32 WeaponIndex)
{
	if (CurrentEquippedWeapon)
	{
		CurrentEquippedWeapon->SetActorHiddenInGame(true);
	}

	if (CurrentWeapons[WeaponIndex])
	{
		CurrentWeapons[WeaponIndex]->SetActorHiddenInGame(false);
	}

	CurrentEquippedWeapon = CurrentWeapons[WeaponIndex];
	CurrentEquippedWeaponIndex = WeaponIndex;

	OnRep_WeaponChanged();
}

void UWeaponComponent::EquipWeapon_Request(const TArray<FWeaponStats>& WeaponStats)
{
	EquipWeapon_Response(WeaponStats);
}


bool UWeaponComponent::CheckWeaponSlotAvailability(int32& EmptyIndex)
{
	EmptyIndex = -1;

	for (int32 i = 0; i < CurrentWeapons.Num(); i++)
	{
		if (CurrentWeapons[i] == nullptr)
		{
			EmptyIndex = i;
			return true;
		}
	}

	return false;
}


void UWeaponComponent::EquipWeapon_Response_Implementation(const TArray<FWeaponStats>& WeaponStats)
{
	int32 AvailableSlot = -1;
	if (CheckWeaponSlotAvailability(AvailableSlot))
	{
		if (CurrentEquippedWeapon)
		{
			CurrentEquippedWeapon->SetActorHiddenInGame(true);
			CurrentEquippedWeapon = nullptr;

			CurrentEquippedWeaponIndex = AvailableSlot;
		}
	}
	else
	{
		DropWeapon_Request();
	}

	// We don't need all weapon stats to have weapon class per se, just the primary one.
	check(WeaponStats[0].WeaponClass);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;

	AActor* Owner = GetOwner();

	CurrentEquippedWeapon =
        GetWorld()->SpawnActor<AWeapon>(
            WeaponStats[0].WeaponClass,
            Owner->GetActorLocation(),
            Owner->GetActorRotation(),
            SpawnInfo
        );

	CurrentEquippedWeapon->SetOwner(Owner);
	CurrentEquippedWeapon->Stats = WeaponStats;

	if (!Character)
	{
		AcquireOwner();
	}

	CurrentEquippedWeapon->CharacterRef = Character;

	CurrentWeapons[CurrentEquippedWeaponIndex] = CurrentEquippedWeapon;

	OnRep_WeaponCollectionChanged();
	OnRep_WeaponChanged();
}

void UWeaponComponent::DropWeapon_Request()
{
	if (CurrentEquippedWeapon)
	{
		DropWeapon_Response();
	}
}

void UWeaponComponent::DropWeapon_Response_Implementation()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;

	const AActor* Owner = GetOwner();

	AWeaponPickup* WeaponPickup = GetWorld()->SpawnActor<AWeaponPickup>(
        DropItemClass,
        Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100,
        Owner->GetActorRotation(),
        SpawnInfo
    );

	WeaponPickup->InitializeWeapon(CurrentEquippedWeapon->Stats);

	CurrentEquippedWeapon->Destroy();

	CurrentWeapons[CurrentEquippedWeaponIndex] = nullptr;
	CurrentEquippedWeapon = nullptr;

	OnRep_WeaponChanged();
	OnRep_WeaponCollectionChanged();
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponComponent, CurrentEquippedWeapon);
	DOREPLIFETIME(UWeaponComponent, CurrentWeapons);

	DOREPLIFETIME(UWeaponComponent, bIsReloading);

}

