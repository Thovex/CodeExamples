// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"


#include "PlayerAnimInstance.generated.h"

class APlayerCharacter;
/**
 * 
 */


UENUM(BlueprintType)
enum class EPlayerWeaponAnimation : uint8 {
	Default							UMETA(DisplayName = "Default"),
    Variant_I						UMETA(DisplayName = "Variant_I"),
    Variant_II						UMETA(DisplayName = "Variant II"),
    Variant_III						UMETA(DisplayName = "Variant III"),
    Variant_IV						UMETA(DisplayName = "Variant IV"),
    Variant_V						UMETA(DisplayName = "Variant V"),
    Variant_VI						UMETA(DisplayName = "Variant VI"),
    Variant_VII						UMETA(DisplayName = "Variant VII"),
    Variant_VIII					UMETA(DisplayName = "Variant VIII"),

};

UCLASS(Blueprintable)
class BITFARERS_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerAnimInstance();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Owner)
	APlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	EPlayerWeaponAnimation WeaponAnimationType;
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetWeaponAnimationType(const EPlayerWeaponAnimation NewType);

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
