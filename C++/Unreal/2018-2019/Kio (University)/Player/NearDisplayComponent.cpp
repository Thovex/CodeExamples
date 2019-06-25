// Copyright 2018-2019 Prickly, All Rights Reserved.

#include "NearDisplayComponent.h"
#include "Player/KioCharacter.h"
#include "Interfaces/Displayable.h"

UNearDisplayComponent::UNearDisplayComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	KioCharacter = Cast<AKioCharacter>( GetOwner() );
}

void UNearDisplayComponent::BeginPlay() {
	Super::BeginPlay();

	KioCharacter = Cast<AKioCharacter>( GetOwner() );

	if ( !KioCharacter ) {
		DestroyComponent();
	}
}

void UNearDisplayComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) {
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	FCloseActorMapTypes ReturnData;
	TArray<AActor*> ValidActors;

	if ( KioCharacter->GetNearFinderComponent()->ReturnCloseActorMapByName( FName( "DisplayAble" ), ReturnData ) ) {
		for ( const TPair<AActor*, FCloseActor>& pair : ReturnData.CloseActorMaps ) {
			if ( pair.Key->GetClass()->ImplementsInterface( UDisplayable::StaticClass() ) ) {
				ValidActors.Add( pair.Key );

				if ( !CurrentDisplayingActors.Contains( pair.Key ) ) {
					CurrentDisplayingActors.Add( pair.Key );
					IDisplayable::Execute_Display( pair.Key, true );
				}
			}
		}
	}

	if ( CurrentDisplayingActors.Num() != ValidActors.Num() ) {
		for ( int32 Index = 0; Index < CurrentDisplayingActors.Num(); Index++ ) {
			if ( !ValidActors.Contains( CurrentDisplayingActors[Index] ) ) {
				IDisplayable::Execute_Display( CurrentDisplayingActors[Index], false );
				CurrentDisplayingActors.RemoveAt( Index );
			}
		}
	}
}
