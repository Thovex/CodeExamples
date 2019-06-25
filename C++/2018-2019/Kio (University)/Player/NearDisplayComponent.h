// Copyright 2018-2019 Prickly, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NearKioFinderComponent.h"
#include "NearDisplayComponent.generated.h"

class AKioCharacter;


UCLASS( ClassGroup = ( Kio ), meta=(BlueprintSpawnableComponent) )
class KIO_API UNearDisplayComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNearDisplayComponent();

protected:
	AKioCharacter* KioCharacter;
	UNearKioFinderComponent* NearKioFinderComponent;
	TArray<AActor*> CurrentDisplayingActors;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
