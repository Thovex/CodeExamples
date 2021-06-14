// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"

#include "BitFarers/Systems/Loot/ItemData.h"
#include "BitFarers/Systems/Loot/Pickups/SlurpPickup.h"
#include "BitFarers/Weapons/Ammo/EAmmoType.h"

#include "ClientItemPickup.generated.h"

/**
 * 
 */
UCLASS()
class BITFARERS_API AClientItemPickup : public ASlurpPickup
{
	GENERATED_BODY()

public:
	AClientItemPickup();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Pickup")
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	int32 ItemCount;

    void SetItemData();

	UFUNCTION(BlueprintCallable, Category="Pickup")
    virtual void InitializePickup(FItemData InItemData, int32 InItemCount, float InIdleTime = 0);
	
protected:
	virtual bool Prerequisites() override;
	virtual void ExecutePickup() override;

private:
	EAmmoType AmmoType;

};
