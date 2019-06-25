// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#include "DebugEditorLineCompToActor.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"


UDebugEditorLineCompToActor::UDebugEditorLineCompToActor() {
	FetchComponents();
}

void UDebugEditorLineCompToActor::BeginPlay() {
	Super::BeginPlay();
}

void UDebugEditorLineCompToActor::FetchComponents() {
	if ( GetOwner() ) {
		for ( UActorComponent* Component : GetOwner()->GetComponents() ) {
			if ( Component ) {
				USceneComponent* ComponentToScene = Cast<USceneComponent>( Component );

				if ( ComponentToScene ) {
					if ( ComponentToScene->GetFName() == StartComponentName ) {
						StartComponent = ComponentToScene;
					}
				}
			}
		}
	}
}

#if WITH_EDITOR
void UDebugEditorLineCompToActor::PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent ) {
	Super::PostEditChangeProperty( PropertyChangedEvent );

	FetchComponents();
}
#endif

void UDebugEditorLineCompToActor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) {
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	const UWorld* World = GetWorld();

	if ( World ) {
		if ( StartComponent && EndActor ) {
			DrawDebugLine( World, StartComponent->GetComponentLocation(), EndActor->GetActorLocation(), DebugLineColor, false, .1F, 0, DebugLineThickness );
		} else {
			if ( Retries < 50 ) {
				FetchComponents();
				Retries++;
			}
		}
	}
}