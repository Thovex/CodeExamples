// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"

#include "PlayerWidget.h"
#include "PlayerCrosshairWidget.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class BITFARERS_API UPlayerCrosshairWidget : public UPlayerWidget
{
	GENERATED_BODY()

public:
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//void HitTarget(const EWeaponDamageType DamageType, float Damage, bool bCritical, const AActor* Actor);

	
protected:
	virtual void NativeConstruct() override;
};
