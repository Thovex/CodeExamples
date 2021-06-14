// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"

#include "BitFarers/Characters/Player/PlayerCharacter.h"
#include "GameFramework/GameMode.h"
#include "NetworkGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BITFARERS_API ANetworkGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ANetworkGameMode(class FObjectInitializer const&);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APlayerCharacter> PlayerClass;

	UPROPERTY()
	TArray<APlayerController*> PlayerControllers;

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

};
