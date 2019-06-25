// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#include "KioGameViewportClient.h"
#include "Runtime/Engine/Classes/Engine/Canvas.h"


void UKioGameViewportClient::PostRender( UCanvas* Canvas ) {
	Super::PostRender( Canvas );

	if ( bFading ) {
		DrawScreenFade( Canvas );
	}
}

void UKioGameViewportClient::ClearFade() {
	bFading = false;

}

void UKioGameViewportClient::Fade( const float Duration, const bool bToBlack ) {
	const UWorld* World = GetWorld();
	if ( World ) {
		bFading = true;
		this->bToBlack = bToBlack;
		FadeDuration = Duration;
		FadeStartTime = World->GetTimeSeconds();
	}
}

void UKioGameViewportClient::DrawScreenFade( UCanvas* Canvas ) {
	if ( bFading ) {
		const UWorld* World = GetWorld();
		if ( World ) {
			const float Time = World->GetTimeSeconds();
			const float Alpha = FMath::Clamp( ( Time - FadeStartTime ) / FadeDuration, 0.f, 1.f );

			if ( Alpha == 1.f && !bToBlack ) {
				bFading = false;
			} else {
				FColor OldColor = Canvas->DrawColor;
				FLinearColor FadeColor = FLinearColor::Black;
				FadeColor.A = bToBlack ? Alpha : 1 - Alpha;
				Canvas->DrawColor = FadeColor.ToFColor( true ); 
				Canvas->DrawTile( Canvas->DefaultTexture, 0, 0, Canvas->ClipX, Canvas->ClipY, 0, 0, Canvas->DefaultTexture->GetSizeX(), Canvas->DefaultTexture->GetSizeY() );
				Canvas->DrawColor = OldColor;
			}
		}
	}
}

