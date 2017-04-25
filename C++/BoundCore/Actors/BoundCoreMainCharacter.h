// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "BoundCoreMainCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(BoundCoreCharacterLog, Log, All);

UCLASS(config = Game)
class ABoundCoreMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABoundCoreMainCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(EditAnywhere)
		AActor* OtherBoundCoreActor;

	bool AbilitySwapReady;
	bool AbilitySwapHasParticle;

protected:

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AbilitySwapCharacter();
	void FindOtherBoundCoreMainCharacters();

private:
	bool FoundOtherPlayerActors = false;
};

