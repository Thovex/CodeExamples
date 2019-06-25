// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#include "DebugEditorLineCompToComp.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

UDebugEditorLineCompToComp::UDebugEditorLineCompToComp() {
	FetchComponents();
}

void UDebugEditorLineCompToComp::BeginPlay() {
	Super::BeginPlay();
}

void UDebugEditorLineCompToComp::FetchComponents() {
	if ( GetOwner() ) {
		for ( UActorComponent* Component : GetOwner()->GetComponents() ) {
			if ( Component ) {
				USceneComponent* ComponentToScene = Cast<USceneComponent>( Component );

				if ( ComponentToScene ) {
					if ( ComponentToScene->GetFName() == StartComponentName ) {
						StartComponent = ComponentToScene;
					}

					if ( ComponentToScene->GetFName() == EndComponentName ) {
						EndComponent = ComponentToScene;
					}
				}
			}
		}
	}
}

#if WITH_EDITOR
void UDebugEditorLineCompToComp::PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent ) {
	Super::PostEditChangeProperty( PropertyChangedEvent );

	FetchComponents();
}
#endif

void UDebugEditorLineCompToComp::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) {
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	const UWorld* World = GetWorld();

	if ( World ) {
		if ( StartComponent && EndComponent ) {
			DrawDebugLine( World, StartComponent->GetComponentLocation(), EndComponent->GetComponentLocation(), DebugLineColor, false, .1F, 0, DebugLineThickness );
		} else {
			if ( Retries < 50 ) {
				FetchComponents();
				Retries++;
			}
		}
	}
}
