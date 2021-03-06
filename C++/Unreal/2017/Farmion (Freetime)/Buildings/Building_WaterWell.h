// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#pragma once

#include "CoreMinimal.h"
#include "Common/AuthorityActor.h"
#include "Components/BoxComponent.h"
#include "Building_WaterWell.generated.h"

/**
 *
 */
UCLASS()
class FARMION_API ABuilding_WaterWell : public AAuthorityActor {
	GENERATED_BODY()

public:
	ABuilding_WaterWell(const FObjectInitializer & ObjectInitializer);

	float GetWaterFillSpeed() const { return WaterFillSpeed; }

protected:
	void ServerTick(float DeltaTime);
	void ClientTick(float DeltaTime);
	UPROPERTY(Replicated)
		bool bWorking = true;

	UPROPERTY(EditAnywhere)
		float WaterFillSpeed = 1;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = WaterArea)
		UBoxComponent* WaterArea;




};
