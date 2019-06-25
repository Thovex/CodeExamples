// Copyright 2018-2019 Prickly, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NearKioFinderComponent.h"
#include "NearOutlineComponent.generated.h"

class AKioCharacter;

UCLASS( ClassGroup = ( Kio ), meta=(BlueprintSpawnableComponent) )
class KIO_API UNearOutlineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNearOutlineComponent();

protected:
	AKioCharacter* KioCharacter;
	UNearKioFinderComponent* NearKioFinderComponent;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
