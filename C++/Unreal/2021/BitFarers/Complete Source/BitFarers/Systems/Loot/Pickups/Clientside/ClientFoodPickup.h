// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"

#include "ClientItemPickup.h"
#include "ClientFoodPickup.generated.h"

class UStatusComponent;

/**
 * 
 */
UCLASS()
class BITFARERS_API AClientFoodPickup : public AClientItemPickup
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
