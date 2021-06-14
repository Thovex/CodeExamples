// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"


#include "ClientItemPickup.h"
#include "BitFarers/Weapons/Ammo/EAmmoType.h"
#include "ClientAmmoPickup.generated.h"

class UAmmoComponent;
/**
 * 
 */
UCLASS()
class BITFARERS_API AClientAmmoPickup : public AClientItemPickup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAmmoType AmmoType;
	
	protected:
	virtual void BeginPlay() override;
	virtual bool Prerequisites() override;
	virtual void ExecutePickup() override;

	virtual void InitializePickup(FItemData InItemData, int32 InItemCount, float InIdleTime) override;

private:
	
	UPROPERTY()
	UAmmoComponent* TargetComponent;
};
