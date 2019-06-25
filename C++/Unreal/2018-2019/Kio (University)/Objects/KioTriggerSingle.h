// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "KioTriggerBase.h"
#include "KioTriggerSingle.generated.h"

/**
 *
 */
UCLASS()
class KIO_API AKioTriggerSingle : public AKioTriggerBase {
	GENERATED_BODY()

public:
	AKioTriggerSingle( const FObjectInitializer& ObjectInitializer );

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Trigger Settings" )
		AKioTriggerEntityBase* TriggerEntity;

protected:
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;
	virtual void Trigger( AActor* HitActor ) override;
};
