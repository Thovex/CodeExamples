// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#pragma once

#include "CoreMinimal.h"
#include "Common/AuthorityActor.h"
#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"
#include "Building_PlantTile.generated.h"

/**
 *
 */

class AObject_GrowingObjectBase;

UCLASS()
class FARMION_API ABuilding_PlantTile : public AAuthorityActor {
	GENERATED_BODY()

public:
	ABuilding_PlantTile(const FObjectInitializer & ObjectInitializer);

	float IncreaseCurrentWaterIfPossible(float BucketWaterAmount);
	float DecreaseCurrentWaterIfPossible(float BucketWaterAmount);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = PlantChild)
		UChildActorComponent* PlantChild;

	AObject_GrowingObjectBase* GetCurrentOwnerGrowingObject() const { return CurrentOwnerGrowingObject; }
	void SetCurrentOwnerGrowingObject(AObject_GrowingObjectBase* val) { CurrentOwnerGrowingObject = val; }
protected:
	void ServerTick(float DeltaTime);
	void ClientTick(float DeltaTime);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = WaterArea)
		UBoxComponent* WaterArea;



	UPROPERTY(EditAnywhere)
		float WaterIntakePerTick = 1;

	UPROPERTY(EditAnywhere)
		float CurrentWater = 0;

	UPROPERTY(EditAnywhere)
		float MaxWater = 500;

	AObject_GrowingObjectBase* CurrentOwnerGrowingObject;
};
