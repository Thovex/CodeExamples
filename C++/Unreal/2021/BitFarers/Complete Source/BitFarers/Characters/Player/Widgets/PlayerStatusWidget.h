// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "PlayerWidget.h"
#include "PlayerStatusWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class BITFARERS_API UPlayerStatusWidget : public UPlayerWidget
{
	GENERATED_BODY()

public:
	/*
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText GetCurrentAmmo_Text() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateHealth(const int32 CurrentHealth, const int32 MaxHealth, const float HealthPercentage);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateShield(const int32 CurrentShield, const int32 MaxShield, const float ShieldPercentage);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateAmmo(const TMap<EAmmoType, float>& AmmoPercentages);
	*/
	
protected:
	//virtual void NativeConstruct() override;

private:

	/*
	// We do not want GC to collect this.
	UPROPERTY()
	UStatusComponent* PlayerStatus;

	UPROPERTY()
	UTraitComponent* PlayerTraits;

	UPROPERTY()
	UPlayerAmmunitionComponent* PlayerAmmo;

	*/
	
	// Delegates must be marked UFUNCTION.

	/*
	UFUNCTION()
	void UpdateHealth_Internal(int32 CurrentHealth, int32 MaxHealth);

	UFUNCTION()
	void UpdateShield_Internal(int32 CurrentShield, int32 MaxShield);

	UFUNCTION()
	void UpdateAmmo_Internal();
	*/

};
