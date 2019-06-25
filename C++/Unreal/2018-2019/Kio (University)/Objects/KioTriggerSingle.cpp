// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#include "KioTriggerSingle.h"
#include "KioTriggerEntityBase.h"

AKioTriggerSingle::AKioTriggerSingle( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer ) {
	PrimaryActorTick.bCanEverTick = true;

	TriggerType = ETriggerType::ETT_SingleTrigger;
}

void AKioTriggerSingle::BeginPlay() {
	Super::BeginPlay();
}

void AKioTriggerSingle::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
}

void AKioTriggerSingle::Trigger( AActor* HitActor ) {
	Super::Trigger(HitActor);

	if ( TriggerEntity ) {
		if ( TriggerEntity->GetClass()->ImplementsInterface( UTriggerable::StaticClass() ) ) {
			ITriggerable::Execute_Trigger( TriggerEntity, this, HitActor );
		}
	} else {
		if ( GEngine ) GEngine->AddOnScreenDebugMessage( -1, 2.0f, FColor::Cyan, TEXT( "No trigger entity set in KioTriggerSingle::Trigger()" ) );
	}
}
