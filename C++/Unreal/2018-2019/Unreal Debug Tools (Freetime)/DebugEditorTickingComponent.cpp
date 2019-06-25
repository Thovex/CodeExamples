// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#include "DebugEditorTickingComponent.h"

UDebugEditorTickingComponent::UDebugEditorTickingComponent() {
	PrimaryComponentTick.bCanEverTick = true;

	bTickInEditor = true;
	bIsEditorOnly = true;

}

void UDebugEditorTickingComponent::BeginPlay() {
	Super::BeginPlay();

	SetComponentTickEnabled( false );
}


void UDebugEditorTickingComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) {
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

}


