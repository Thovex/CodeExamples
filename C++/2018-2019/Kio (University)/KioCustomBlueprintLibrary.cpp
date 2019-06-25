// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#include "KioCustomBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Framework/KioGameViewportClient.h"

AKioCharacter* UKioCustomBlueprintLibrary::ReturnKioCharacter( const UObject* WorldContextObject ) {
	if ( GEngine ) {
		UWorld* World = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::LogAndReturnNull );

		if ( World ) {
			return Cast<AKioCharacter>( UGameplayStatics::GetPlayerCharacter( World, 0 ) );
		}

		UE_LOG( LogTemp, Error, TEXT( "Unable to receive UWorld. Returning from ReturnKioCharacter()" ) );
	}
	UE_LOG( LogTemp, Error, TEXT( "Unable to receive GEngine. Returning from ReturnKioCharacter()" ) );
	return nullptr;
}

void UKioCustomBlueprintLibrary::Fade( const UObject* WorldContextObject, const float Duration, const bool bToBlack ) {
	if ( GEngine ) {
		UWorld* World = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::LogAndReturnNull );

		if ( World ) {
			UKioGameViewportClient* GameViewportClient = Cast<UKioGameViewportClient>( World->GetGameViewport() );
			if ( GameViewportClient ) {
				GameViewportClient->Fade( Duration, bToBlack );
			}
		}
	}
}

bool UKioCustomBlueprintLibrary::VectorSmallerCheck( FVector V1, FVector V2 ) {
	if ( FloatSmallerCheck( V1.X, V2.X ) && FloatSmallerCheck( V1.Y, V2.Y ) && FloatSmallerCheck( V1.Z, V2.Z ) ) return true;
	return false;
}

bool UKioCustomBlueprintLibrary::FloatSmallerCheck( float F1, float F2 ) {
	if ( F1 < F2 ) return true;
	return false;
}

float UKioCustomBlueprintLibrary::SetScale( float CurrentValue, float OldMinScale, float OldMaxScale, float NewMinScale, float NewMaxScale ) {

	return ( ( ( CurrentValue - OldMinScale ) * ( NewMaxScale - NewMinScale ) ) / ( OldMaxScale - OldMinScale ) ) + NewMinScale;
}

float UKioCustomBlueprintLibrary::VectorToVectorPercentageCheck( FVector V1, FVector V2, FVector OriginScale ) {
	float X1 = V1.X;
	float Y1 = V1.Y;
	float Z1 = V1.Z;

	float X2 = V2.X;
	float Y2 = V2.Y;
	float Z2 = V2.Z;

	float XPercentage = SetScale( X1, OriginScale.X, X2, 0, 100 );
	float YPercentage = SetScale( Y1, OriginScale.Y, Y2, 0, 100 );
	float ZPercentage = SetScale( Z1, OriginScale.Z, Z2, 0, 100 );

	return ( XPercentage + YPercentage + ZPercentage ) / 3;
}

float UKioCustomBlueprintLibrary::SetScalePure( float CurrentValue, float OldMinScale, float OldMaxScale, float NewMinScale, float NewMaxScale ) {

	return ( ( ( CurrentValue - OldMinScale ) * ( NewMaxScale - NewMinScale ) ) / ( OldMaxScale - OldMinScale ) ) + NewMinScale;
}
