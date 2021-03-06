// Copyright Baer and Hoggo Games


#include "NetworkPlayerController.h"
#include "BitFarers/Utilities/AccessorLibrary.h"
#include "BitFarers/Traveling/PersistenceManagerTwo.h"
#include "BitFarers/Characters/Player/PlayerCharacter.h"
#include "GameFramework/PlayerState.h"

void ANetworkPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Cast<APlayerCharacter>(InPawn)->PlayerIndex = PlayerState->GetPlayerId();
}
