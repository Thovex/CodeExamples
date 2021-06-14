// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "BitFarers/Systems/Status/Traits/Trait.h"
#include "BitFarers/Weapons/EElementalType.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, int32, CurrentHealth, int32, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChanged, int32, CurrentShield, int32, MaxShield);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BITFARERS_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatusComponent();
	
	UFUNCTION() void OnRep_CurrentHealthChanged();
	UFUNCTION() void OnRep_CurrentShieldChanged();

	UPROPERTY(BlueprintAssignable) FOnHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable) FOnShieldChanged OnShieldChanged;
	UPROPERTY(BlueprintAssignable) FOnDeath OnDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traits")
	TArray<UTrait*> Traits;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentHealthChanged, EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentHealth;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentShieldChanged, EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentShield;

	UFUNCTION(BlueprintCallable)
	UTrait* InitializeTrait(const ETraitType TraitType, const float Base, const float Min, const float Max);

	UFUNCTION(BlueprintCallable)
    UTrait* GetTraitPointer(const ETraitType TraitType);
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
    void Heal_CurrentHealth_Server(float Heal);

	UFUNCTION(BlueprintCallable, Server, Reliable)
    void Heal_CurrentShield_Server(float Heal);
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
    void SetHealthShield_Server(const float Health, const float Shield);

	UFUNCTION(BlueprintCallable, Server, Reliable)
    void TakeDamage_Server(bool IsPlayerInstigator, bool Critical, float Damage);

	UFUNCTION(BlueprintCallable, BlueprintPure)
    float GetNumberPercentageFromTotal(float Number) ;
	
	UFUNCTION(BlueprintCallable, Category = "Damage")
	float TakeDamage(const EElementalType ElementalType, const float Damage, const AActor* DamageInstigator,
		const UActorComponent* ImpactComponent, const FVector ImpactPoint);

	UFUNCTION(BlueprintCallable)
	void UpdateHealth_Broadcast(float PreviousValue, float CurrentValue);
	
	UFUNCTION(BlueprintCallable)
    void UpdateShield_Broadcast(float PreviousValue, float CurrentValue);

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

private:
	FTimerHandle SecondTickHandle;

	void SecondTick();
	void InitializeStatus();

	bool bInternalDeath = false;
};
