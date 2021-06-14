#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "BitFarers/Systems/NetworkGameState.h"

#include "AccessorLibrary.generated.h"

class APlayerCharacter;

UCLASS( Blueprintable )
class UAccessorLibrary : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject")) 
    static ANetworkGameState* GetNetworkGameState(const UObject* WorldContextObject);
    
};
