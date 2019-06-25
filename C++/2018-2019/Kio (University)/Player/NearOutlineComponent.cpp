// Copyright 2018-2019 Prickly, All Rights Reserved.

#include "NearOutlineComponent.h"
#include "Player/KioCharacter.h"
#include "Interfaces/Outlineable.h"

UNearOutlineComponent::UNearOutlineComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	KioCharacter = Cast<AKioCharacter>( GetOwner() );
}

void UNearOutlineComponent::BeginPlay() {
	Super::BeginPlay();

	KioCharacter = Cast<AKioCharacter>( GetOwner() );

	if ( !KioCharacter ) {
		DestroyComponent();
	}
}

void UNearOutlineComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) {
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	FCloseActorMapTypes ReturnData;

	if ( KioCharacter->GetNearFinderComponent()->ReturnCloseActorMapByName( FName( "UseAble" ), ReturnData ) ) {
		ReturnData.CloseActorMaps.ValueSort( FSortCloseActorByDistance() );

		TArray<AActor*> CurrentActors;
		AActor* ClosestActor = NULL;
		float ClosestAngle = MAX_FLOAT;

		for ( const TPair<AActor*, FCloseActor>& pair : ReturnData.CloseActorMaps ) {
			if ( pair.Key->GetClass()->ImplementsInterface( UOutlineable::StaticClass() ) ) {
				CurrentActors.Add( pair.Key );

				if ( pair.Value.Angle < ClosestAngle ) {
					ClosestActor = pair.Key;
					ClosestAngle = pair.Value.Angle;

				}
			}
		}

		if ( ClosestActor != nullptr ) {

			CurrentActors.Remove( ClosestActor );
			IOutlineable::Execute_Outline( ClosestActor, true );
		}

		KioCharacter->SetCurrentSelectedActor( ClosestActor );

		for ( AActor* Actor : CurrentActors ) {
			IOutlineable::Execute_Outline( Actor, false );
		}

	}
}
