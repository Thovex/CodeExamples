// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"

#include "BitFarers/Characters/Player/Customization/PlayerCustomizationSave.h"
#include "Engine/GameInstance.h"
#include "NetworkGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BITFARERS_API UNetworkGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	UPlayerCustomizationSave* GetPlayerCustomizationSave();
};
