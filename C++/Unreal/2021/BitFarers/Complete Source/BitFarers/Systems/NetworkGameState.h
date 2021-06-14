// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"

#include "BitFarers/Characters/Player/PlayerCharacter.h"
#include "BitFarers/Traveling/PersistenceManagerTwo.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameState.h"
#include "NetworkGameState.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BITFARERS_API ANetworkGameState : public AGameState
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Table", meta = (AllowPrivateAccess = "true"))
	UDataTable* LootTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Table", meta = (AllowPrivateAccess = "true"))
	UDataTable* WeaponTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Association Table", meta = (AllowPrivateAccess = "true"))
	UDataTable* AssociationTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	APersistenceManager* PersistenceManager;
	

public:
	UFUNCTION(BlueprintCallable)
	APlayerCharacter* GetPlayerFromId(int32 PlayerId);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	APersistenceManager* GetPersistenceManager();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

private:

};
