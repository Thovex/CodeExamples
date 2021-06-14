// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "ItemPickup.h"
#include "FoodPickup.generated.h"

class UStatusComponent;
/**
 * 
 */
UCLASS()
class BITFARERS_API AFoodPickup : public AItemPickup
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual bool Prerequisites() override;
	virtual void ExecutePickup() override;

private:
	UPROPERTY()
	UStatusComponent* TargetComponent;

};
