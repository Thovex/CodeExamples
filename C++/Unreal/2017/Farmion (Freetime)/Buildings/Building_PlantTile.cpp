// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#include "Building_PlantTile.h"
#include "UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DualQuat.h"


ABuilding_PlantTile::ABuilding_PlantTile(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer) {
	StaticMeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("RootMesh"));
	WaterArea = CreateDefaultSubobject<UBoxComponent>(TEXT("WaterArea"));
	WaterArea->SetupAttachment(StaticMeshComponent);
	PlantChild = CreateDefaultSubobject<UChildActorComponent>(TEXT("PlantChild"));
	PlantChild->SetupAttachment(StaticMeshComponent);

	bReplicates = true;
	bAlwaysRelevant = true;
}

float ABuilding_PlantTile::IncreaseCurrentWaterIfPossible(float BucketWaterAmount) {
	int wateringAmount = BucketWaterAmount;

	if(BucketWaterAmount > WaterIntakePerTick) {
		wateringAmount = WaterIntakePerTick;
	}

	if(MaxWater - CurrentWater < wateringAmount) {
		CurrentWater = MaxWater;
		return MaxWater - CurrentWater;
	} else {
		CurrentWater += wateringAmount;
		return wateringAmount;
	}
}

float ABuilding_PlantTile::DecreaseCurrentWaterIfPossible(float plantTakeAmount) {
	if(plantTakeAmount > CurrentWater) {
		int CurrentSave = CurrentWater;
		CurrentWater = 0;
		return CurrentSave;
	} else {
		CurrentWater -= plantTakeAmount;
		return plantTakeAmount;
	}
}

void ABuilding_PlantTile::ServerTick(float DeltaTime) {
	DrawDebugStringAtActor(FString::FromInt(CurrentWater));
}

void ABuilding_PlantTile::ClientTick(float DeltaTime) {

}
