// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "TraitModifier.h"
#include "ETraitType.h"
#include "Trait.generated.h"

class UStatusComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChange, float, PreviousValue, float, CurrentValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnModifierAdded, FName, Name, float, Duration, float, Power, EModifierType, Type);

UCLASS(Blueprintable)
class BITFARERS_API UTrait : public UObject
{
public:
	GENERATED_BODY()

	UTrait();

	/* Properties */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trait")
	ETraitType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trait Stats")
	float Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trait Stats")
	float Min;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trait Stats")
	float Max;

	UPROPERTY()
	UStatusComponent* Outer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trait")
	TArray<FTraitModifier> TraitModifiers;

	/* Functions */

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetBase() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMin() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMax() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetModified();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMaximumValue();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMinimumValue();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetModifierCount() const;

	UFUNCTION(BlueprintCallable)
    void Initialize(const ETraitType InType, const float InBase, const float InMin, const float InMax, UStatusComponent* InOuter);
	
	UFUNCTION(BlueprintCallable)
	FGuid AddNewModifier(FTraitModifier Modifier);

	UFUNCTION(BlueprintCallable)
	void RemoveModifierByGUID(const FGuid Guid);

	/* Delegates */
	UPROPERTY(BlueprintAssignable)
	FOnChange OnChange;
	
	UPROPERTY(BlueprintAssignable)
	FOnModifierAdded OnModifierAdded;

protected:
	virtual bool IsSupportedForNetworking() const override;
	
	
private:
	float CalculateModified();
};
