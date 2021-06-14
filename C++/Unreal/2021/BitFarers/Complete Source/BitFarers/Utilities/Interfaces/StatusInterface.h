// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BitFarers/Systems/Status/StatusComponent.h"
#include "StatusInterface.generated.h"

UINTERFACE(BlueprintType)
class BITFARERS_API UStatusInterface : public UInterface
{
	GENERATED_BODY()

	UStatusInterface();
};

/**
* 
*/
class BITFARERS_API IStatusInterface
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintImplementableEvent, BlueprintCallable, Category = "HasStatus Interface")
	void InitializeStatus();
	
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "HasStatus Interface")
	float TakeDamage(EElementalType ElementalType, float Damage, const AActor* DamageInstigator,
		const UActorComponent* ImpactComponent, FVector ImpactPoint);

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "HasStatus Interface")
	bool IsPlayer();
};
