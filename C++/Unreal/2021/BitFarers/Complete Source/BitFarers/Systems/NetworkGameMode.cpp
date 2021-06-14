// Copyright Baer and Hoggo Games


#include "NetworkGameMode.h"


#include "NetworkGameState.h"
#include "GameFramework/GameStateBase.h"


ANetworkGameMode::ANetworkGameMode(FObjectInitializer const&)
{
	
}

void ANetworkGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayerControllers.Add(NewPlayer);
	
}

void ANetworkGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	APlayerController* Controller = Cast<APlayerController>(Exiting);
	PlayerControllers.Remove(Controller);
}
