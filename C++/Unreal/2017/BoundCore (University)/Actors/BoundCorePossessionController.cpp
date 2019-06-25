// Fill out your copyright notice in the Description page of Project Settings.

#include "BoundCoreMain.h"
#include "BoundCorePossessionController.h"
#include "Runtime/ActorSequence/Private/ActorSequencePrivatePCH.h"
#include "BoundCoreMainCharacter.h"

DEFINE_LOG_CATEGORY(BoundCorePossessionControllerLog);

ABoundCorePossessionController::ABoundCorePossessionController() {
	PrimaryActorTick.bCanEverTick = true;
	ReadyForPossession = false;
}

void ABoundCorePossessionController::BeginPlay() {
	Super::BeginPlay();

	for (TActorIterator<ABoundCoreMainCharacter> It(GetWorld()); It; ++It) {
		AActor* BoundCoreActor = *It;
		BoundCoreActors.Add(BoundCoreActor);
	}

	for (TActorIterator<ABoundCoreMainCharacter> It(GetWorld()); It; ++It) {
		BoundCoreActorsMainCharacter.Add(Cast<ABoundCoreMainCharacter>(*It));
	}
}

void ABoundCorePossessionController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	CheckMainCharactersSwapStatus();
}

void ABoundCorePossessionController::CheckMainCharactersSwapStatus() {
	int ReadyCount = 0;

	for (int i = 0; i < BoundCoreActors.Num(); i++) {
		if (BoundCoreActorsMainCharacter[i]->AbilitySwapReady) {
			ReadyCount++;

			if (!BoundCoreActorsMainCharacter[i]->AbilitySwapHasParticle) {
				if (ReadyParticle) {
					UGameplayStatics::SpawnEmitterAttached(
						ReadyParticle,
						BoundCoreActors[i]->FindComponentByClass<USkeletalMeshComponent>(),
						FName("AbilitySwapCharacterParticle"),
						FVector(0, 0, 0),
						FRotator(0, 90, 0),
						EAttachLocation::KeepRelativeOffset,
						true
					);

					BoundCoreActorsMainCharacter[i]->AbilitySwapHasParticle = true;
				}
			}
		}
	}

	if (ReadyCount == 2) {
		FTimerHandle ParticleTimerTillReset;
		GetWorldTimerManager().ClearTimer(ParticleTimerTillReset);
		GetWorldTimerManager().SetTimer(ParticleTimerTillReset, this, &ABoundCorePossessionController::ResetParticles, ParticleTimeTillReset, false, -1.0f);

		ReadyCount = 0;

		for (int i = 0; i < BoundCoreActors.Num(); i++) {
			BoundCoreActorsMainCharacter[i]->AbilitySwapReady = false;
		}
	}
}

TArray<FRotator> ABoundCorePossessionController::GetOldRotationsAndReset() {
	TArray <FRotator> OldRotations = TArray<FRotator>();

	int BoundCoreActorsNum = BoundCoreActors.Num();

	for (int i = 0; i < BoundCoreActorsNum; i++) {
		OldRotations.Add(BoundCoreActorsMainCharacter[i]->GetActorRotation());

		FRotator NewRotation = FRotator(0, 0, 0);
		BoundCoreActors[i]->SetActorRotation(NewRotation);
	}

	return OldRotations;
}

void ABoundCorePossessionController::SetNewRotations(TArray<FRotator> OldRotations) {
	int BoundCoreActorsNum = BoundCoreActors.Num();

	for (int i = 0; i < BoundCoreActorsNum; i++) {
		BoundCoreActors[i]->SetActorRotation(OldRotations[i]);
	}

}

void ABoundCorePossessionController::ResetParticles() {
	int BoundCoreActorsNum = BoundCoreActors.Num();

	for (int i = 0; i < BoundCoreActorsNum; i++) {
		TArray<UParticleSystemComponent*> children;
		BoundCoreActors[i]->GetComponents<UParticleSystemComponent>(children);

		// if has particlesystems
		if (children.Num() > 0) {

			// Mark child particlesystem for kill
			children[0]->DestroyComponent();
			BoundCoreActorsMainCharacter[i]->AbilitySwapHasParticle = false;
		}
	}

	AbilitySwapCharacter();
}

void ABoundCorePossessionController::AbilitySwapCharacter() {
	TArray<ACharacter*> BoundCoreCharacters;
	TArray<AController*> BoundCoreCharacterControllers;

	int BoundCoreActorsNum = BoundCoreActors.Num();

	for (int i = 0; i < BoundCoreActorsNum; i++) {
		ACharacter* NewCharacter = Cast<ACharacter>(BoundCoreActors[i]);
		BoundCoreCharacters.Add(NewCharacter);
	}

	for (int i = 0; i < BoundCoreActorsNum; i++) {
		AController* NewController = BoundCoreCharacters[i]->GetController();
		BoundCoreCharacterControllers.Add(NewController);
	}

	for (int i = 0; i < BoundCoreActorsNum; i++) {
		BoundCoreCharacterControllers[i]->UnPossess();
	}

	TArray<FRotator> OldRotations = GetOldRotationsAndReset();

	for (int i = 0; i < BoundCoreActorsNum; i++) {
		if (i == 0) {
			BoundCoreCharacterControllers[i]->Possess(BoundCoreCharacters[1]);
		}
		else if (i == 1) {
			BoundCoreCharacterControllers[i]->Possess(BoundCoreCharacters[0]);
		}
	}

	SetNewRotations(OldRotations);
}



