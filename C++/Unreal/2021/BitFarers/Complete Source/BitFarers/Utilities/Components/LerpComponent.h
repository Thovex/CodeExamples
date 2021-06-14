// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"

#include "LerpData.h"
#include "Components/ActorComponent.h"
#include "LerpComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BITFARERS_API ULerpComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULerpComponent();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasVectorInstance(const USceneComponent* TargetComponent, int32& Index);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasRotatorInstance(const USceneComponent* TargetComponent, int32& Index);

	UFUNCTION(BlueprintCallable)
	void AddVectorLerpInstance(USceneComponent* TargetComponent, const float InterpSpeed, const FVector LerpEndTarget,
	                           const ELerpSpace LerpSpace);

	UFUNCTION(BlueprintCallable)
	void AddRotatorLerpInstance(USceneComponent* TargetComponent, const float InterpSpeed, const FRotator LerpEndTarget,
                           const ELerpSpace LerpSpace);

private:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	TArray<FVectorLerpData> Internal_VectorLerp;
	TArray<FRotatorLerpData> Internal_RotatorLerp;
};
