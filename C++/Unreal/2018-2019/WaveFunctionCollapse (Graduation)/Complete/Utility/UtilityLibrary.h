// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityLibrary.generated.h"

/**
 *
 */

UENUM( BlueprintType )
enum class ERoadSide : uint8 {
	ERS_Forward 	UMETA( DisplayName = "Forward" ),
	ERS_Right 		UMETA( DisplayName = "Right" ),
	ERS_Back		UMETA( DisplayName = "Back" ),
	ERS_Left		UMETA( DisplayName = "Left" )
};


UCLASS()
class ANDROMEDARESONANCE_API UUtilityLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:

	UFUNCTION( BlueprintPure, meta = ( DisplayName = "ToIntVector (Vector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast ), Category = "Math|Conversions" )
		static FIntVector Conv_VectorToIntVector( const FVector& InVector );

	UFUNCTION( BlueprintPure, meta = ( DisplayName = "ToIntVector (Rotator)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast ), Category = "Math|Conversions" )
		static FIntVector Conv_RotatorToIntVector( const FRotator& InRotator );

	UFUNCTION( BlueprintPure, meta = ( DisplayName = "ToRotator (IntVector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast ), Category = "Math|Conversions" )
		static FRotator Conv_IntVectorToRotator( const FIntVector& InIntVector );

	UFUNCTION( BlueprintPure, meta = ( DisplayName = "Divide Vector (By IntVector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast ), Category = "Math|Conversions" )
		static FVector Divide_VectorByIntVector( const FVector& InVector, const FIntVector& InIntVector );

	UFUNCTION( BlueprintPure, meta = ( DisplayName = "ToIntVector (Vector)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast ), Category = "Math|Conversions" )
		static FVector Vector_AddIntVector( const FVector& InVector, const FIntVector& InIntVector );

	UFUNCTION( BlueprintCallable, Category = "Math", meta = ( WorldContext = "WorldContextObject" ) )
		static float SetScale( float CurrentValue, float OldMinScale, float OldMaxScale, float NewMinScale, float NewMaxScale );

	UFUNCTION( BlueprintCallable, BlueprintPure, meta = ( BlueprintThreadSafe ), Category = "Math", meta = ( WorldContext = "WorldContextObject" ) )
		static float SetScalePure( float CurrentValue, float OldMinScale, float OldMaxScale, float NewMinScale, float NewMaxScale );

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Road Conversion")
		static bool EqualByteArray(  TArray<uint8> A,  TArray<uint8> B );

};
