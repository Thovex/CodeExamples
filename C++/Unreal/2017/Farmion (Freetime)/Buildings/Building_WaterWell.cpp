// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#include "Building_WaterWell.h"
#include "UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DualQuat.h"


ABuilding_WaterWell::ABuilding_WaterWell(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer) {
	NetPriority = 0;
	NetUpdateFrequency = 100;
	MinNetUpdateFrequency = 2;
	bReplicateMovement = true;
	bNetLoadOnClient = true;
	bReplicates = true;
	StaticMeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("RootMesh"));
	WaterArea = CreateDefaultSubobject<UBoxComponent>(TEXT("WaterArea"));
	WaterArea->SetupAttachment(StaticMeshComponent);
}


void ABuilding_WaterWell::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	DOREPLIFETIME(ABuilding_WaterWell, bWorking);
}

void ABuilding_WaterWell::ServerTick(float DeltaTime) {

}

void ABuilding_WaterWell::ClientTick(float DeltaTime) {

}


