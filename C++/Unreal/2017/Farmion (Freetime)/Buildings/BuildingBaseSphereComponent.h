// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "BuildingBaseSphereComponent.generated.h"

/**
 * 
 */
UCLASS()
class FARMION_API UBuildingBaseSphereComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UBuildingBaseSphereComponent();
	
	UPROPERTY(Replicated)
		TArray<FName> BuildingSphereTags;
	
	UFUNCTION(reliable, NetMulticast, WithValidation)
		void SetDetails(FName SocketName, FTransform SocketTransform, UStaticMeshComponent* MeshToAttachTo);
	
	void SetDetails_Implementation(FName SocketName, FTransform SocketTransform, UStaticMeshComponent * MeshToAttachTo);

};
