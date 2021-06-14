// Copyright Baer and Hoggo Games


#include "PersistenceManagerTwo.h"
#include "ActZoneLevelsRow.h"
#include "TravelPortal.h"
#include "TravelPortalLocation.h"
#include "ZoneManager.h"
#include "BitFarers/Macros.h"
#include "BitFarers/Utilities/AccessorLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

APersistenceManager::APersistenceManager()
{
	bReplicates = true;
}


void APersistenceManager::DoCleansing(bool bAuthority)
{
	TArray<AActor*> WorldActors;
	UGameplayStatics::GetAllActorsOfClass(this, AActor::StaticClass(), WorldActors);

	for (AActor* Actor : WorldActors)
	{
		TSubclassOf<AActor> ActorClass = Actor->GetClass();

		bool IsChildOfFilter = false;
		
		for (auto FilterClass : CryoFilter)
		{
			if (ActorClass == FilterClass)
			{
				IsChildOfFilter = true;
				break;
			}
			
			if (ActorClass->IsChildOf(FilterClass))
			{
				IsChildOfFilter = true;
				break;
			}
		}
		
		if (IsChildOfFilter)
		{
			// Check if actor is not already being destroyed.
			if (!Actor->IsPendingKill() && !Actor->IsPendingKillPending())
			{
				// Git em
				Actor->Destroy();
			}
		}
	}
}

void APersistenceManager::DoWorldTransitionCleansing_Implementation()
{
	DoCleansing(true);
	DoWorldTransitionCleansingComplete();
}

void APersistenceManager::DoWorldTransitionCleansingComplete_Implementation()
{
	DoCleansing(false);
}

void APersistenceManager::DoTravel_Implementation()
{
	OnStartTravel.Broadcast();
}

void APersistenceManager::DoTravelEnd_Server_Implementation()
{
	DoTravelEnd_Multicast();
}

void APersistenceManager::DoTravelEnd_Multicast_Implementation()
{
	OnPostTravel.Broadcast();
}

void APersistenceManager::DoTravelStart_Server_Implementation()
{
	DoTravelStart_Multicast();
}

void APersistenceManager::DoTravelStart_Multicast_Implementation()
{
	OnPreTravel.Broadcast();
}

void APersistenceManager::StartMap()
{
	if (HasAuthority())
	{
		AZoneManager* ZoneManager = Cast<AZoneManager>(
         UGameplayStatics::GetActorOfClass(this, AZoneManager::StaticClass()));

		if (ZoneManager)
		{
			ZoneManager->InitializeZone(Intro, CurrentLevelName, ZoneAct, ZoneType, ZoneDifficulty);
			SpawnPortals();
			
			ZoneManager->SetPortals(Portals);
		} else
		{
			PRINT("Zone does not have manager!");
		}
	}
}

void APersistenceManager::SpawnPortals()
{
	if (HasAuthority())
	{
		UWorld* World = GetWorld();
	
		TArray<AActor*> FoundPortals;
		UGameplayStatics::GetAllActorsOfClass(this, ATravelPortalLocation::StaticClass(), FoundPortals);

		Portals.Empty();
		
		for (auto& Portal : FoundPortals)
		{
			ATravelPortalLocation* PortalLocation = Cast<ATravelPortalLocation>(Portal);
			
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.bNoFail = true;

			ATravelPortal* SpawnedPortal =
				World->SpawnActor<ATravelPortal>(PortalClass, Portal->GetActorTransform(), SpawnParameters);

			FName RowName = NAME_None;
			switch (PortalLocation->Act)
			{
			case EAct::Island: { RowName = FName("Island"); } break;
			case EAct::Desert: { RowName = FName("Desert"); } break;
			case EAct::Underworld: {RowName = FName("Underworld"); } break;
			default: { PRINT("Act Name Inconclusive"); } ;
			}

			FString Context;
			const FActZoneLevelsRow* DataRow = MapData->FindRow<FActZoneLevelsRow>(RowName, Context, true);
			const FActZoneLevels ActZones = DataRow->LevelData.FindChecked(PortalLocation->ZoneType);

			int32 RandomMapIdx = FMath::RandRange(0, ActZones.Levels.Num() - 1);

			int32 FailSafeIncrement = 0;
			
			while (CurrentLevelName == ActZones.Levels[RandomMapIdx].UniqueName)
			{
				RandomMapIdx = FMath::RandRange(0, ActZones.Levels.Num() - 1);

				FailSafeIncrement++;

				if (FailSafeIncrement > 1000)
				{
					PRINT("Failsafe Increment reached! Current map can only take the same map as a new map");
					break;
				}
			}

			SpawnedPortal->SetIntro(ActZones.Levels[RandomMapIdx].IntroData);
			SpawnedPortal->SetLevels(ActZones.Levels[RandomMapIdx].UniqueName , ActZones.Levels[RandomMapIdx].LevelRef, CurrentLevel);
			SpawnedPortal->SetZone(PortalLocation->Act, PortalLocation->ZoneType, ActZones.Levels[RandomMapIdx].AllowedDifficulties);

			SpawnedPortal->SetReady.AddDynamic(this, &APersistenceManager::UnreadyPortals);
			SpawnedPortal->PortalSetup();
			
			Portals.Add(SpawnedPortal);

			Portal->Destroy();
		
		}
	}
}

void APersistenceManager::UnreadyPortals(int32 PlayerId, AActor* Portal)
{
	const ATravelPortal* TravelPortal = Cast<ATravelPortal>(Portal);

	for (auto& ExistingPortal : Portals)
	{
		if (ExistingPortal != TravelPortal)
		{
			ExistingPortal->ForceUnready(PlayerId);
		}
	}
	
}

void APersistenceManager::BeginPlay()
{
	Super::BeginPlay();	
	
	StartMap();
}

