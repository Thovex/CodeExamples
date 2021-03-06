// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FarmionCharacterAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class FARMION_API UFarmionCharacterAnimInstance : public UAnimInstance {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Woodcutting")
		void WoodcutHit(APawn* ThePawn);


	UFUNCTION(BlueprintCallable, Category = "Mining")
		void MiningHit(APawn* ThePawn);

protected:
	virtual void NativeInitializeAnimation() override;

	//APawn * OwningPawn;

	//virtual void NativeInitializeAnimation() override;
	//virtual void NativeUpdateAnimation(float DeltaTimeX) override;

};
