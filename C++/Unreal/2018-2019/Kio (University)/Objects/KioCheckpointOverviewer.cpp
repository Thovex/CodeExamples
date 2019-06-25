// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#include "KioCheckpointOverviewer.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Debug/DrawDebugText.h"

AKioCheckpointOverviewer::AKioCheckpointOverviewer() {
	PrimaryActorTick.bCanEverTick = true;
	bIsEditorOnlyActor = true;
}

void AKioCheckpointOverviewer::BeginPlay() {
	Super::BeginPlay();

	SetActorTickEnabled( false );
}

/* This tick gets disabled in BeginPlay! Editor-Only Tick. */
void AKioCheckpointOverviewer::Tick( float DeltaSeconds ) {
	if ( bDebugMode ) {
		DrawDebug();
	}
}

void AKioCheckpointOverviewer::DrawDebug() {

	//DrawDebugCharacter( GetWorld(), 'E',GetActorLocation(), FColor::Red, false, -1.0F, 0, 3 );


	for ( TObjectIterator<AKioCheckpoint> Itr; Itr; ++Itr ) {
		AKioCheckpoint *KioCheckpoint = *Itr;


		if ( GetWorld() ) {
			if ( KioCheckpoint ) {
				if ( KioCheckpoint->GetActorLocation() != FVector::ZeroVector ) {
					if ( bDrawDebugCheckpointLines ) {
						DrawDebugLine( GetWorld(), GetActorLocation(), KioCheckpoint->GetActorLocation(), DebugLineColor, false, .1F, 1, DebugCheckpointLineThickness );
					}

					if ( bDrawDebugCheckpointBox ) {
						FVector Orgin, BoxExtent;
						KioCheckpoint->GetActorBounds( true, Orgin, BoxExtent );
						DrawDebugBox( GetWorld(), KioCheckpoint->GetActorLocation(), BoxExtent, DebugBoxColor, false, .1F, 1, DebugCheckpointBoxThickness );

						//Todo add FQuat rotation, but eh, probs not using these anyway.
					}

					if ( bDrawDebugText ) {

						DrawDebugText( GetWorld(), KioCheckpoint->CheckpointName, KioCheckpoint->GetActorLocation() - FVector( 0, 0, 75 ),
									   DebugTextColor, false, .1F, 1, DebugTextThickness, DebugTextSize, bRotateDebugTextToCamera,
									   bTextHasMaxDistance, DebugMaximumTextDistance, bTextCapitalize );

						DrawDebugText( GetWorld(), "INDEX " + FString::FromInt( KioCheckpoint->CheckpointIndex ), KioCheckpoint->GetActorLocation() - FVector( 0, 0, -75 ),
									   DebugTextColor, false, .1F, 1, DebugTextThickness, DebugTextSize, bRotateDebugTextToCamera,
									   bTextHasMaxDistance, DebugMaximumTextDistance, bTextCapitalize );
					}
				}
			}
		}
	}
}

void AKioCheckpointOverviewer::AddCheckpoint( int32 CheckpointIndex, AKioCheckpoint* CheckpointRef ) {
	if ( CheckpointRef ) {
		CheckpointDictionary.Add( CheckpointIndex, CheckpointRef );

		BindDelegate( CheckpointRef );
		UE_LOG( LogKio, Warning, TEXT( "Added checkpoint index: %d to CheckpointOverview" ), CheckpointIndex );
	}
}

bool AKioCheckpointOverviewer::ShouldTickIfViewportsOnly() const {
	return true;
}

void AKioCheckpointOverviewer::BindDelegate( AKioCheckpoint* CheckpointRef ) {
	CheckpointRef->OnCheckpointHit.AddDynamic( this, &AKioCheckpointOverviewer::TestFunction );
}

void AKioCheckpointOverviewer::TestFunction() {
	if ( GEngine ) GEngine->AddOnScreenDebugMessage( -1, 2.0f, FColor::Cyan, TEXT( "ya ya " ) );
}