// Copyright Baer and Hoggo Games



#include "PlayerStatusWidget.h"
#include "BitFarers/Characters/Player/PlayerCharacter.h"


/*
void UPlayerStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RETURN_IF_NULLPTR(PlayerCharacter);

	PlayerStatus = PlayerCharacter->GetStatus_Implementation();
	PlayerTraits = PlayerCharacter->GetTraits_Implementation();

	PlayerAmmo = PlayerCharacter->GetAmmo();

	if (PlayerStatus && PlayerAmmo && PlayerTraits)
	{
		PlayerStatus->OnHealthChanged.AddDynamic(this, &UPlayerStatusWidget::UpdateHealth_Internal);
		PlayerStatus->OnShieldChanged.AddDynamic(this, &UPlayerStatusWidget::UpdateShield_Internal);
		PlayerAmmo->OnAmmoChanged.AddDynamic(this, &UPlayerStatusWidget::UpdateAmmo_Internal);

		UpdateHealth_Internal(
			PlayerStatus->CurrentHealth, PlayerTraits->GetTraitData(ETraitType::MaxHealth).GetPower());

		UpdateShield_Internal(
			PlayerStatus->CurrentShield, PlayerTraits->GetTraitData(ETraitType::MaxShield).GetPower());

		UpdateAmmo_Internal();

	} else
	{
		LOG_SCREENMSG("Missing PlayerStatus, PlayerAmmo or PlayerTraits in PlayerStatusWidget");
	}
}

void UPlayerStatusWidget::UpdateHealth_Internal(const int32 CurrentHealth, const int32 MaxHealth)
{
	const float HealthPercentage = static_cast<float>(CurrentHealth)/static_cast<float>(MaxHealth);
	const float ClampedHealthPercentage = FMath::Clamp(HealthPercentage, 0.0F, 1.0F);
	UpdateHealth(CurrentHealth, MaxHealth, ClampedHealthPercentage);
}

void UPlayerStatusWidget::UpdateShield_Internal(const int32 CurrentShield, const int32 MaxShield)
{
	const float ShieldPercentage = static_cast<float>(CurrentShield)/static_cast<float>(MaxShield);
	const float ClampedShieldPercentage = FMath::Clamp(ShieldPercentage, 0.0F, 1.0F);

	// Note this is divided by 2 to adhere to "50%" of the HealthBar. (With a check for DIVIDE_BY_ZERO)
	UpdateShield(CurrentShield, MaxShield, ClampedShieldPercentage > 0 ? ClampedShieldPercentage / 2 : 0);
}

void UPlayerStatusWidget::UpdateAmmo_Internal()
{
	RETURN_IF_NULLPTR(PlayerAmmo);

	TMap<EWeaponAmmunition, float> PercentageMap;

	for (auto& AmmoType : PlayerAmmo->CurrentAmmunition)
	{
		const float AmmoPercentage = static_cast<float>(AmmoType.CurrentAmmo)/static_cast<float>(AmmoType.MaxAmmo);
		PercentageMap.Add(AmmoType.AmmunitionType, AmmoPercentage);
	}

	UpdateAmmo(PercentageMap);
}

FText UPlayerStatusWidget::GetCurrentAmmo_Text() const
{
	if (!PlayerCharacter || !PlayerCharacter->GetWeapon() || !PlayerCharacter->GetAmmo())
	{
		// Return Disconnected text in case of invalid requirements. 
		return FText::FromString("-- Disconnected --");
	}

	const AWeaponBase* EquippedWeapon = PlayerCharacter->GetWeapon()->CurrentEquippedWeapon;
	
	// Return Current ammo based on current Equipped weapon values
	if (EquippedWeapon)
	{
		const int32 TotalAmmo =
			PlayerCharacter->GetAmmo()->GetTotalAmmo_Remaining(EquippedWeapon->WeaponStats.AmmunitionType);

		switch (EquippedWeapon->WeaponStats.AmmunitionType)
		{
			case EWeaponAmmunition::Energy:
				{
					// Return energy weapon style (MaxAmmo only)
					return FText::FromString(FString::Printf(TEXT("%d"), TotalAmmo));
				}
			default:
				{
					// Return default weapon style (Current of Weapon/Max of Weapon/Absolute Total).
					return FText::FromString(FString::Printf( TEXT( "%d/%d/%d" ),
						EquippedWeapon->CurrentAmmo,
						EquippedWeapon->WeaponStats.WeaponMaxAmmoClip,
						TotalAmmo));
				}
		}
		
	}
	
	// Return "" Text for no weapon equipped.
	return FText();
}

*/