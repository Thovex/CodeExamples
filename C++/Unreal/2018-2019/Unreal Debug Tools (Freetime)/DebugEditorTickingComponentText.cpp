// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#include "DebugEditorTickingComponentText.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Debug/DrawDebugText.h"

void UDebugEditorTickingComponentText::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) {
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	DrawDebug();
}

void UDebugEditorTickingComponentText::DrawDebug() {
	if ( GetWorld() ) {
		DrawDebugText( GetWorld(), DisplayText, GetOwner()->GetActorLocation() + DisplayOffset,
					   DebugTextColor, false, .1F, 1, DebugTextThickness, DebugTextSize, bRotateDebugTextToCamera,
					   bTextHasMaxDistance, DebugMaximumTextDistance, bTextCapitalize
		);
	}
}
