// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "BitFarers/Systems/Loot/ItemData.h"
#include "BitFarers/Systems/Loot/Pickups/SlurpPickup.h"
#include "BitFarers/Weapons/Ammo/EAmmoType.h"


#include "ItemPickup.generated.h"

/**
 * 
 */
UCLASS()
class BITFARERS_API AItemPickup : public ASlurpPickup
{
	GENERATED_BODY()

public:
	AItemPickup();

	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_SetItemData, BlueprintReadWrite, Category="Pickup")
	FItemData ItemData;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category="Pickup")
	int32 ItemCount;

	UFUNCTION()
	void OnRep_SetItemData();

	UFUNCTION(BlueprintCallable, Category="Pickup")
	virtual void InitializePickup(FItemData InItemData, int32 InItemCount, float InIdleTime = 0);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Pickup")
    void InitializePickup_Response(FItemData InItemData, int32 InItemCount);
	
protected:
	virtual bool Prerequisites() override;
	virtual void ExecutePickup() override;

	virtual void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const override;

private:
	EAmmoType AmmoType;
};
