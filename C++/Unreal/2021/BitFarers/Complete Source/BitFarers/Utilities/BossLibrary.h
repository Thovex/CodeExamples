// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"

#include "BossLibrary.generated.h"

class APlayerCharacter;

USTRUCT(BlueprintType)
struct FVectorList
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> Vectors;
};

UCLASS( Blueprintable )
class UBossLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject")) 
    static TMap<int32, FVectorList> CircleCast(const UObject* WorldContextObject, USceneComponent* BaseComponent,
    	int32 Divider, int32 ForRadial, int32 PosIncr, int32 RadIncr);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static FString VectorArrayToFString(const UObject* WorldContextObject, const TArray<FVector> InVectors);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static FString VectorToFString(const UObject* WorldContextObject, const FVector InVector);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static TArray<FVector> FStringToVectorArray(const UObject* WorldContextObject, const FString InString);
	
    
};
