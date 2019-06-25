// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#include "DebugEditorWorldText.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Debug/DrawDebugText.h"

ADebugEditorWorldText::ADebugEditorWorldText() {
	Transform = CreateDefaultSubobject<USceneComponent>( FName( "Transform" ) );
	RootComponent = Transform;

	bIsEditorOnlyActor = true;

	PrimaryActorTick.bCanEverTick = true;
}

void ADebugEditorWorldText::BeginPlay() {
	Super::BeginPlay();

	SetActorTickEnabled( false );

}

void ADebugEditorWorldText::Tick( float DeltaTime ) {
	if ( bDebugMode ) {
		DrawDebug();
	}
}

void ADebugEditorWorldText::DrawDebug() {

	if ( GetWorld() ) {
		DrawDebugText( GetWorld(), DisplayText, GetActorLocation(),
					   DebugTextColor, false, .1F, 1, DebugTextThickness, DebugTextSize, bRotateDebugTextToCamera,
					   bTextHasMaxDistance, DebugMaximumTextDistance, bTextCapitalize 
		);
	}
}

bool ADebugEditorWorldText::ShouldTickIfViewportsOnly() const {
	return true;
}


