// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#include "KioTriggerBase.h"
#include "Player/KioCharacter.h"
#include "Interfaces/Triggerable.h"
#include "KioTriggerEntityBase.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"


AKioTriggerBase::AKioTriggerBase( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer ) {
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>( this, TEXT( "Transform" ) );

	TriggerIdentityData = ObjectInitializer.CreateDefaultSubobject<UKioTriggerIdentity>( this, TEXT( "TriggerIdentityData" ) );
	TriggerIdentityData->SetupAttachment( RootComponent );

	TriggerBoxCollision = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>( this, TEXT( "TriggerBoxCollision" ) );
	TriggerBoxCollision->SetupAttachment( RootComponent );

	TriggerBoxCollision->SetCollisionResponseToAllChannels( ECR_Ignore );
	TriggerBoxCollision->SetCollisionResponseToChannel( ECC_Pawn, ECR_Overlap );

}

UBoxComponent* AKioTriggerBase::GetTriggerCollision() const {
	return TriggerBoxCollision;
}

FTriggerIdentityData AKioTriggerBase::GetTriggerIdentity() const {
	return TriggerIdentityData->TriggerIdentityData;
}

void AKioTriggerBase::BeginPlay() {
	Super::BeginPlay();
	BindDelegates();
	TriggerBeginCheck();
}

void AKioTriggerBase::TriggerBeginCheck() {
	TArray<AActor*> OverlappingActors;
	TriggerBoxCollision->GetOverlappingActors( OverlappingActors );

	for ( int32 Index = 0; Index < OverlappingActors.Num(); Index++ ) {
		for ( int32 JIndex = 0; JIndex < ActorClassesToCallTrigger.Num(); JIndex++ ) {
			if ( OverlappingActors[Index]->IsA( ActorClassesToCallTrigger[JIndex] ) ) {
				Trigger( OverlappingActors[Index] );
			}
		}
	}
}

void AKioTriggerBase::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
}

void AKioTriggerBase::Trigger( AActor* HitActor ) {
	OnTriggerHit.Broadcast( HitActor );
}

void AKioTriggerBase::BindDelegates() {
	if ( TriggerBoxCollision ) {
		TriggerBoxCollision->OnComponentBeginOverlap.AddDynamic( this, &AKioTriggerBase::BeginOverlap );
		TriggerBoxCollision->OnComponentEndOverlap.AddDynamic( this, &AKioTriggerBase::EndOverlap );
	}
}

bool AKioTriggerBase::ClassCheck( AActor* OtherActor ) {
	if ( ActorClassesToCallTrigger.Num() == 0 ) return false;

	for ( int32 Index = 0; Index < ActorClassesToCallTrigger.Num(); Index++ ) {
		if ( ActorClassesToCallTrigger[Index] != NULL ) {
			if ( OtherActor->IsA( ActorClassesToCallTrigger[Index] ) ) {
				return true;
			}
		} 
	}
	return false;
}

void AKioTriggerBase::BeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult ) {
	if ( ClassCheck( OtherActor )) {
		Trigger( OtherActor );
	}
}

void AKioTriggerBase::EndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex ) {

}