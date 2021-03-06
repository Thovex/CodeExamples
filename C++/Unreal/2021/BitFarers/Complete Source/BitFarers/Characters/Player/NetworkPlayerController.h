// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NetworkPlayerController.generated.h"

class AWeaponBase;

/**
 *
 */

UCLASS()
class BITFARERS_API ANetworkPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;

};
