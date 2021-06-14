// Copyright Baer and Hoggo


#include "TravelPortal.h"
#include "PersistenceManagerTwo.h"
#include "BitFarers/Macros.h"
#include "BitFarers/Systems/NetworkGameState.h"
#include "BitFarers/Utilities/AccessorLibrary.h"
#include "Net/UnrealNetwork.h"


ATravelPortal::ATravelPortal()
{
	bReplicates = true;
}

void ATravelPortal::BeginPlay()
{
	Super::BeginPlay();
}
void ATravelPortal::SetIntro(FActZoneLevelIntroData IntroData)
{
	Intro = IntroData;
}

void ATravelPortal::SetZone(const EAct InAct, const EZoneType InZoneType, const TArray<EZoneDifficulty> Difficulties)
{
	Act = InAct;
	ZoneType = InZoneType;
	ZoneDifficulty = DetermineDifficulty(Difficulties);
}

void ATravelPortal::SetLevels(FName LoadLevelName, const TSoftObjectPtr<UWorld> LoadLevel, TSoftObjectPtr<UWorld> UnloadLevel)
{
	LoadName = LoadLevelName;
	Load = LoadLevel;
	Unload = UnloadLevel;

}

void ATravelPortal::ForceUnready(const int32 PlayerId)
{
	ReadyStatus.Add(PlayerId, false);
}

void ATravelPortal::OnRep_Act()
{
	PortalSetup();
}

void ATravelPortal::OnRep_Type()
{
	PortalSetup();
}

void ATravelPortal::OnRep_Difficulty()
{
	PortalSetup();
}

void ATravelPortal::OnRep_Name()
{
	PortalSetup();
}

void ATravelPortal::DoButtonClick_Implementation(int32 PlayerId, int32 FunctionalIndex)
{
	// We just use this to declare it as clickable.
}


void ATravelPortal::Server_Click_Implementation(const int32 PlayerId)
{		
	if (ReadyStatus.Contains(PlayerId))
	{
		ReadyStatus[PlayerId] = !ReadyStatus[PlayerId];	
	} else
	{
		ReadyStatus.Add(PlayerId, true);
	}

	SetReady.Broadcast(PlayerId, this);
	CheckReady();
}

void ATravelPortal::CheckReady()
{
	if (HasAuthority())
	{
		ANetworkGameState* NetworkGameState =  UAccessorLibrary::GetNetworkGameState(this);
		const int32 PlayerCount = NetworkGameState->PlayerArray.Num();

		if (ReadyStatus.Num() == PlayerCount)
		{
			bool bReady = true;
			
			for (const auto Ready : ReadyStatus)
			{
				if (!Ready.Value)
				{
					bReady = false;
					break;
				}
			}

			if (bReady)
			{
				NetworkGameState->GetPersistenceManager()->DoTravelStart_Server();
				
				UWorld* World = GetWorld();
				World->GetTimerManager().SetTimer(TravelTimer, this, &ATravelPortal::DoTravel, .25F, false);
			}
		} 
	}
}

void ATravelPortal::DoTravel()
{
	Server_DoTravel();
}

void ATravelPortal::Server_DoTravel_Implementation()
{
	ANetworkGameState* GameState = UAccessorLibrary::GetNetworkGameState(this);
	APersistenceManager* PersistenceManager = GameState->GetPersistenceManager();
	PersistenceManager->DoWorldTransitionCleansing();
	
	Unload = PersistenceManager->CurrentLevel;
	
	PersistenceManager->CurrentLevel = Load;
	PersistenceManager->CurrentLevelName = LoadName;
	
	PersistenceManager->Load = Load;
	PersistenceManager->Unload = Unload;

	PersistenceManager->ZoneAct = Act;
	PersistenceManager->ZoneType = ZoneType;
	PersistenceManager->ZoneDifficulty = ZoneDifficulty;
	PersistenceManager->Intro = Intro;
	
	PersistenceManager->DoTravel();
}


EZoneDifficulty ATravelPortal::DetermineDifficulty(const TArray<EZoneDifficulty> Difficulties) const
{
	const auto Val = FMath::RandRange(0.0F, 100.0F);

	switch (Difficulties.Num())
	{
		default:
			{
				PRINT("Issue in determining zone difficulty!");
				return EZoneDifficulty::Normal;
			}
		case 1: { return Difficulties[0]; }
		case 2: { return Val <= 75 ? Difficulties[0] : Difficulties[1];}
		case 3: { return Val <= 50 ? Difficulties[0] : Val <= 85 ? Difficulties[1] : Difficulties[2];}
		case 4: { return Val <= 40 ? Difficulties[0] : Val <= 65 ? Difficulties[1] : Val <= 85 ? Difficulties[2] : Difficulties[3];}
	}
}

void ATravelPortal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATravelPortal, Act);
	DOREPLIFETIME(ATravelPortal, ZoneType);
	DOREPLIFETIME(ATravelPortal, ZoneDifficulty);
	DOREPLIFETIME(ATravelPortal, LoadName);

}
