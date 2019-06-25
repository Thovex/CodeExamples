// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#include "KioCheckpoint.h"
#include "Player/KioCharacter.h"
#include "KioCheckpointOverviewer.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"

AKioCheckpoint::AKioCheckpoint( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer ) {
	PrimaryActorTick.bCanEverTick = false;

	// Set root transform (scene) component.
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>( this, TEXT( "Transform" ) );

	// Create checkpoint collision box component.
	CheckpointCollision = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>( this, TEXT( "CheckpointCollision" ) );
	CheckpointCollision->SetupAttachment( RootComponent );
}

FString AKioCheckpoint::GetCheckpointName() const {
	return CheckpointName;
}

UBoxComponent* AKioCheckpoint::GetCheckpointCollision() const {
	return CheckpointCollision;
}

void AKioCheckpoint::BeginPlay() {
	Super::BeginPlay();

	RegisterCheckpoint();
	BindDelegates();
}

void AKioCheckpoint::RegisterCheckpoint() {
	for ( TObjectIterator<AKioCheckpointOverviewer> Itr; Itr; ++Itr ) {
		AKioCheckpointOverviewer *KioCheckpointOverviewer = *Itr;
		KioCheckpointOverviewer->AddCheckpoint( CheckpointIndex, this );
	}
}

void AKioCheckpoint::BindDelegates() {
	if ( CheckpointCollision ) {
		// Link OnBeginOverlap to custom function.
		CheckpointCollision->OnComponentBeginOverlap.AddDynamic( this, &AKioCheckpoint::BeginOverlap );
	}
}

void AKioCheckpoint::BeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult ) {
	if ( OtherActor->IsA( AKioCharacter::StaticClass() ) ) {
		OnCheckpointHit.Broadcast();
	}
}