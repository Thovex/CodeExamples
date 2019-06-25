// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#pragma once

#include "Kio.h"
#include "World/Triggers/KioTriggerBase.h"
#include "Triggerable.generated.h"

/**
 *
 */

UINTERFACE( BlueprintType )
class KIO_API UTriggerable : public UInterface {
	GENERATED_BODY()

		UTriggerable( const FObjectInitializer & ObjectInitializer );
};
class KIO_API ITriggerable {
	GENERATED_BODY()

public:

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Triggerable Interface" )
		void Trigger(AKioTriggerBase* Trigger, AActor* TriggeredActor);
};