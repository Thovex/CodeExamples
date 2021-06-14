// Copyright Baer and Hoggo

#pragma once

#include "CoreMinimal.h"


#include "EAct.h"
#include "EZoneType.h"
#include "GameFramework/Actor.h"
#include "TravelPortalLocation.generated.h"

UCLASS()
class BITFARERS_API ATravelPortalLocation : public AActor
{
	GENERATED_BODY()

public:
	ATravelPortalLocation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	EAct Act;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn="true"))
	EZoneType ZoneType;

protected:
	virtual void BeginPlay() override;
};
