// Copyright Baer and Hoggo Games


#include "NetworkGameState.h"
#include "GameFramework/PlayerState.h"
#include "BitFarers/Characters/Player/PlayerCharacter.h"
#include "Net/UnrealNetwork.h"


APlayerCharacter* ANetworkGameState::GetPlayerFromId(int32 PlayerId)
{
    for(auto& Player : PlayerArray)
    {
        if (Player->GetPlayerId() == PlayerId)
        {
            return Cast<APlayerCharacter>(Player->GetPawn());
        }
    }

    return nullptr;
}

APersistenceManager* ANetworkGameState::GetPersistenceManager()
{
    return PersistenceManager;
}

void ANetworkGameState::BeginPlay()
{
    Super::BeginPlay();
}

void ANetworkGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ANetworkGameState, PersistenceManager)
}
