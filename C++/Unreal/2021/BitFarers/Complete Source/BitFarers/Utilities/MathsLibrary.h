#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathsLibrary.generated.h"

UCLASS( Blueprintable )
class UMathsLibrary : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

    public:

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
