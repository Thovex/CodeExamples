// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"

#include "BitFarers/Utilities/Interfaces/ButtonClickable.h"
#include "GameFramework/Actor.h"

#include "ButtonActor.generated.h"

UCLASS()
class BITFARERS_API AButtonActor : public AActor, public IButtonClickable
{
	GENERATED_BODY()
	
public:	
	AButtonActor();

	UFUNCTION(BlueprintCallable)
	virtual void DoButtonClick_Implementation(int32 PlayerId, int32 FunctionalIndex) override;

};
