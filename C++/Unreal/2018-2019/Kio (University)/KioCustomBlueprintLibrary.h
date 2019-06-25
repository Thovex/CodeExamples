// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Player/KioCharacter.h"
#include "KioCustomBlueprintLibrary.generated.h"

UCLASS( Blueprintable )
class KIO_API UKioCustomBlueprintLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintPure, Category = "Kio Character", meta = ( WorldContext = "WorldContextObject" ) )
		static class AKioCharacter* ReturnKioCharacter( const UObject* WorldContextObject );

	UFUNCTION( BlueprintCallable, Category = "Fade", meta = ( WorldContext = "WorldContextObject" ) )
		static void Fade( const UObject* WorldContextObject, const float Duration, const bool bToBlack );

	UFUNCTION( BlueprintCallable, Category = "Math", meta = ( WorldContext = "WorldContextObject" ) )
		static bool VectorSmallerCheck( FVector V1, FVector V2 );

	UFUNCTION( BlueprintCallable, Category = "Math", meta = ( WorldContext = "WorldContextObject" ) )
		static bool FloatSmallerCheck( float F1, float F2 );

	UFUNCTION( BlueprintCallable, Category = "Math", meta = ( WorldContext = "WorldContextObject" ) )
		static float VectorToVectorPercentageCheck( FVector V1, FVector V2, FVector OriginScale );

	UFUNCTION( BlueprintCallable, Category = "Math", meta = ( WorldContext = "WorldContextObject" ) )
		static float SetScale( float CurrentValue, float OldMinScale, float OldMaxScale, float NewMinScale, float NewMaxScale );

	UFUNCTION( BlueprintCallable, BlueprintPure, meta = (BlueprintThreadSafe), Category = "Math", meta = ( WorldContext = "WorldContextObject" ) )
		static float SetScalePure( float CurrentValue, float OldMinScale, float OldMaxScale, float NewMinScale, float NewMaxScale );
};
