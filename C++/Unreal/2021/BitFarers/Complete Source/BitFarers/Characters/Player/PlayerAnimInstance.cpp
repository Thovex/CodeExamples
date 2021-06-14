// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "BitFarers/Macros.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
    
}

void UPlayerAnimInstance::SetWeaponAnimationType_Implementation(const EPlayerWeaponAnimation NewType)
{
    WeaponAnimationType = NewType;
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
}
