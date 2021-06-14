// Copyright Baer and Hoggo Games


#include "StatusComponent.h"

#include "BitFarers/Utilities/Interfaces/AmmoInterface.h"
#include "BitFarers/Systems/Status/Traits/ETraitType.h"
#include "Net/UnrealNetwork.h"

UStatusComponent::UStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);


	/*
	
	// Percentage
	Traits.Add(FTrait(ETraitType::CriticalHitChance, 10, 0, 100));
	Traits.Add(FTrait(ETraitType::CriticalHitDamage, 150, 0, 1500));
	Traits.Add(FTrait(ETraitType::DodgeChancePercentage, 0, 0, 85));

	// As multiplier 
	Traits.Add(FTrait(ETraitType::Accuracy, 1, 0, 5));
	Traits.Add(FTrait(ETraitType::RateOfFire, 1, 0, 5));

	// % -100 (takes % damage life on hit) to 100 (gives % damage life on hit)
	Traits.Add(FTrait(ETraitType::AllDamage, 100, -1000, 1000));

	// As multiplier
	Traits.Add(FTrait(ETraitType::CooldownReduction, 1, -10, 10));
	Traits.Add(FTrait(ETraitType::ReloadSpeed, 1, 0.25, 5));

	// Percentage on kill
	Traits.Add(FTrait(ETraitType::ExperienceGain, 100, 0, 1000));
	Traits.Add(FTrait(ETraitType::MoneyGain, 100, 0, 1000));

	// % -100 (takes % damage life on hit) to 100 (gives % damage life on hit)
	Traits.Add(FTrait(ETraitType::LifeSteal, 0, -100, 100));

	// X per second
	Traits.Add(FTrait(ETraitType::HealthRegen, 0, -10, 10));
	Traits.Add(FTrait(ETraitType::ShieldRegen, 0, -10, 20));
	Traits.Add(FTrait(ETraitType::EnergyRegeneration, 1, 0, 10));

	// Actual counts
	Traits.Add(FTrait(ETraitType::MaximumHealth, 100, 1, 5000));
	Traits.Add(FTrait(ETraitType::MaximumShield, 0, 0, 5000));

	// Actual counts
	Traits.Add(FTrait(ETraitType::MovementSpeed, 100, 25, 1500));

	// Actual counts
	Traits.Add(FTrait(ETraitType::JumpCount, 1, 0, 5));
	Traits.Add(FTrait(ETraitType::DodgeCount, 1, 0, 5));
	Traits.Add(FTrait(ETraitType::UtilityCount, 1, 0, 10));

	// Actual counts
	Traits.Add(FTrait(ETraitType::MaxNormalAmmoCapacity, 512, 0, 512 * 4));
	Traits.Add(FTrait(ETraitType::MaxPenetrationAmmoCapacity, 256, 0, 256 * 4));
	Traits.Add(FTrait(ETraitType::MaxEnergyAmmoCapacity, 128, 0, 128 * 4));
	Traits.Add(FTrait(ETraitType::MaxExplosiveAmmoCapacity, 32, 0, 32 * 4));
	Traits.Add(FTrait(ETraitType::MaxRadioactiveAmmoCapacity, 16, 0, 16 * 4));

	// Percentage
	Traits.Add(FTrait(ETraitType::EnergyResistance, 0, -100, 100));
	Traits.Add(FTrait(ETraitType::ExplosionResistance, 0, -100, 100));
	Traits.Add(FTrait(ETraitType::FireResistance, 0, -100, 100));
	Traits.Add(FTrait(ETraitType::IceResistance, 0, -100, 100));
	Traits.Add(FTrait(ETraitType::PhysicalResistance, 0, -100, 100));
	Traits.Add(FTrait(ETraitType::PoisonResistance, 0, -100, 100));
	Traits.Add(FTrait(ETraitType::SlowResistance, 0, -100, 100));
	*/
}
UTrait* UStatusComponent::InitializeTrait(const ETraitType TraitType, const float Base, const float Min, const float Max)
{
	for (auto& ExistingTrait : Traits)
	{
		if (ExistingTrait->Type == TraitType)
		{
			ExistingTrait->Base = Base;
			ExistingTrait->Min = Min;
			ExistingTrait->Max = Max;

			ExistingTrait->TraitModifiers.Empty();

			return ExistingTrait;
		}
	}
	
	UTrait* NewTrait = NewObject<UTrait>();
	NewTrait->Initialize(TraitType, Base, Min, Max, this);
	Traits.Add(NewTrait);
	return NewTrait;
}

void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(SecondTickHandle, this, &UStatusComponent::SecondTick, 1, true, -1);
}

float UStatusComponent::TakeDamage(const EElementalType ElementalType, const float Damage,
                                   const AActor* DamageInstigator, const UActorComponent* ImpactComponent,
                                   const FVector ImpactPoint)
{
	const float PreviousHealth = CurrentHealth;
	
	CurrentHealth -= Damage;

	if (CurrentHealth != PreviousHealth)
	{
		OnRep_CurrentHealthChanged();	
	}
	
	return Damage;
}

void UStatusComponent::UpdateHealth_Broadcast(float PreviousValue, float CurrentValue)
{
	if (CurrentHealth > CurrentValue)
	{
		SetHealthShield_Server(CurrentValue, CurrentShield);
	}
	
	OnRep_CurrentHealthChanged();
}

void UStatusComponent::UpdateShield_Broadcast(float PreviousValue, float CurrentValue)
{
	if (CurrentShield > CurrentValue)
	{
		SetHealthShield_Server(CurrentShield, CurrentValue);
	}
	
	OnRep_CurrentShieldChanged();
}

void UStatusComponent::InitializeStatus()
{
	UTrait* MaxHealthData = GetTraitPointer(ETraitType::MaximumHealth);
	UTrait* MaxShieldData = GetTraitPointer(ETraitType::MaximumShield);

	if (MaxHealthData && MaxShieldData)
	{
		SetHealthShield_Server(
			MaxHealthData->GetModified(),
			MaxShieldData->GetModified()
		);
	}
}

UTrait* UStatusComponent::GetTraitPointer(const ETraitType TraitType)
{
	for (auto& TraitData : Traits)
	{
		if (TraitData)
		{
			if (TraitData->Type == TraitType)
			{
				return TraitData;
			}
		}
	}
	return nullptr;
}

float UStatusComponent::GetNumberPercentageFromTotal(const float Number)
{
	return Number/(GetTraitPointer(ETraitType::MaximumHealth)->GetModified()
		+ GetTraitPointer(ETraitType::MaximumShield)->GetModified()) * 100;
}

void UStatusComponent::Heal_CurrentShield_Server_Implementation(float Heal)
{

	UTrait* MaxShieldData = GetTraitPointer(ETraitType::MaximumShield);

	if (MaxShieldData)
	{
		const float MaxShield = MaxShieldData->GetModified();

		if (CurrentShield + Heal <= MaxShield)
		{
			CurrentShield += Heal;
		}
		
		if (CurrentShield > MaxShield)
		{
			CurrentShield = MaxShield;
		}
	}

	OnRep_CurrentShieldChanged();
}

void UStatusComponent::Heal_CurrentHealth_Server_Implementation(float Heal)
{

	UTrait* MaxHealthData = GetTraitPointer(ETraitType::MaximumHealth);

	if (MaxHealthData)
	{
		const float MaxHealth = MaxHealthData->GetModified();

		if (CurrentHealth + Heal <= MaxHealth)
		{
			CurrentHealth += Heal;
		}

		if (CurrentHealth > MaxHealth)
		{
			CurrentHealth = MaxHealth;
		}
	}

	OnRep_CurrentHealthChanged();
}

void UStatusComponent::SecondTick()
{
	UTrait* MaxHealthData = GetTraitPointer(ETraitType::MaximumHealth);
	UTrait* HealthRegenerationData = GetTraitPointer(ETraitType::HealthRegen);

	if (MaxHealthData && HealthRegenerationData)
	{
		if (HealthRegenerationData->GetModified() != 0)
		{
			if (CurrentHealth < MaxHealthData->GetModified())
			{
				Heal_CurrentHealth_Server(HealthRegenerationData->GetModified());
			}
		}
	}

	UTrait* MaxShieldData = GetTraitPointer(ETraitType::MaximumShield);
	UTrait* ShieldRegenerationData = GetTraitPointer(ETraitType::ShieldRegen);

	if (MaxShieldData && ShieldRegenerationData)
	{
		if (ShieldRegenerationData->GetModified() != 0)
		{
			if (CurrentShield < MaxShieldData->GetModified())
			{
				Heal_CurrentShield_Server(ShieldRegenerationData->GetModified());
			}
		}
	}

	UTrait* EnergyRegen = GetTraitPointer(ETraitType::EnergyRegeneration);
	if (EnergyRegen)
	{
		if (EnergyRegen->GetModified() != 0)
		{
			UAmmoComponent* AmmoComponent = IAmmoInterface::Execute_RetrieveAmmoComponent(GetOwner());

			if (AmmoComponent)
			{
				if (!AmmoComponent->IsAmmoFull(EAmmoType::Energy))
				{
					AmmoComponent->AddAmmo(EAmmoType::Energy, EnergyRegen->GetModified());
				}
			}
		}
	}
}

void UStatusComponent::OnRep_CurrentHealthChanged()
{
	UTrait* MaxHealthData = GetTraitPointer(ETraitType::MaximumHealth);
	
	if (MaxHealthData) {
		OnHealthChanged.Broadcast(CurrentHealth, MaxHealthData->GetModified());

		if (CurrentHealth <= 0 )
		{
			if (!bInternalDeath)
			{
				bInternalDeath = true;
				OnDeath.Broadcast();
			}
		}
	}
}

void UStatusComponent::OnRep_CurrentShieldChanged()
{
	UTrait* MaxShieldData = GetTraitPointer(ETraitType::MaximumShield);
	
	if (MaxShieldData) {
		OnShieldChanged.Broadcast(CurrentShield, MaxShieldData->GetModified());
	}
}

void UStatusComponent::SetHealthShield_Server_Implementation(const float Health, const float Shield)
{
	CurrentHealth = Health;
	CurrentShield = Shield;

	OnRep_CurrentHealthChanged();
	OnRep_CurrentShieldChanged();
}

void UStatusComponent::TakeDamage_Server_Implementation(bool IsPlayerInstigator, bool Critical, float Damage)
{
	if (CurrentShield > 0)
	{
		CurrentShield -= Damage;
		if (CurrentShield < 0)
		{
			CurrentHealth -= FMath::Abs(CurrentShield);

			OnRep_CurrentHealthChanged();
			CurrentShield = 0;
		}

		OnRep_CurrentShieldChanged();
	}
	else
	{
		CurrentHealth -= Damage;
		OnRep_CurrentHealthChanged();
	}
}

void UStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStatusComponent, CurrentHealth);
	DOREPLIFETIME(UStatusComponent, CurrentShield);

}

void UStatusComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	for (auto& Ptr : Traits)
	{
		if (Ptr)
		{
			Ptr->RemoveFromRoot();
		}
	}

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}
