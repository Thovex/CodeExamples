// Copyright Baer and Hoggo

#include "ZoneManager.h"

#include "TravelPortal.h"
#include "BitFarers/Macros.h"
#include "Net/UnrealNetwork.h"

AZoneManager::AZoneManager()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

void AZoneManager::InitializeZone(const FActZoneLevelIntroData Intro, const FName Name, const EAct Act, const EZoneType Type, const EZoneDifficulty Difficulty)
{
	ZoneName = Name;
	ZoneAct = Act;
	ZoneType = Type;
	ZoneDifficulty = Difficulty;
	ZoneIntro = Intro;

	BeginZone();
}

void AZoneManager::SetPortals(TArray<ATravelPortal*> InPortals)
{	
	Portals = InPortals;
}

void AZoneManager::BeginZone_Implementation()
{
	OnZoneBegin.Broadcast();
}

void AZoneManager::EndZone_Implementation()
{
	OnZoneFinished.Broadcast();

	for (auto Portal : Portals)
	{
		RETURN_IF_NULLPTR(Portal);
		Portal->PortalOpen();
	}
}

void AZoneManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AZoneManager, Portals);
}

