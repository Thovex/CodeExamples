// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BoundCoreMainCharacter.h"
#include "ParticleDefinitions.h"
#include "BoundCorePossessionController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(BoundCorePossessionControllerLog, Log, All);


UCLASS()
class BOUNDCOREMAIN_API ABoundCorePossessionController : public AActor
{
	GENERATED_BODY()

public:
	ABoundCorePossessionController();

	UPROPERTY(EditAnywhere)
		UParticleSystem* ReadyParticle;

	UPROPERTY(EditAnywhere)
		float ParticleTimeTillReset;

	bool ReadyForPossession;

	TArray<AActor*> BoundCoreActors;
	TArray<ABoundCoreMainCharacter*> BoundCoreActorsMainCharacter;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	void AbilitySwapCharacter();

private:
	void CheckMainCharactersSwapStatus();
	TArray<FRotator> GetOldRotationsAndReset();
	void SetNewRotations(TArray<FRotator> OldRotations);
	void ResetParticles();
};
