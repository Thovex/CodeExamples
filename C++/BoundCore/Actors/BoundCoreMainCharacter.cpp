// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BoundCoreMain.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "BoundCoreMainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/ActorSequence/Private/ActorSequencePrivatePCH.h"

DEFINE_LOG_CATEGORY(BoundCoreCharacterLog);


ABoundCoreMainCharacter::ABoundCoreMainCharacter() {
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	AbilitySwapReady = false;
	AbilitySwapHasParticle = false;
}

void ABoundCoreMainCharacter::BeginPlay() {
	Super::BeginPlay();

}

void ABoundCoreMainCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!FoundOtherPlayerActors) {
		FindOtherBoundCoreMainCharacters();
	}
}

void ABoundCoreMainCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Ability_SwapCharacter", IE_Pressed, this, &ABoundCoreMainCharacter::AbilitySwapCharacter);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABoundCoreMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABoundCoreMainCharacter::MoveRight);

}

void ABoundCoreMainCharacter::AbilitySwapCharacter() {
	AbilitySwapReady = true;
}

void ABoundCoreMainCharacter::FindOtherBoundCoreMainCharacters() {

	TArray<AActor*> BoundCoreActors;

	for (TActorIterator<ABoundCoreMainCharacter> It(GetWorld()); It; ++It) {
		AActor* BoundCoreActor = *It;
		BoundCoreActors.Add(BoundCoreActor);
	}

	if (BoundCoreActors.Num() == 2) {
		FoundOtherPlayerActors = true;

		int IndexOfRemoval;

		for (int i = 0; i < BoundCoreActors.Num(); i++) {
			if (this == BoundCoreActors[i]) {
				IndexOfRemoval = i;
			}
		}

		BoundCoreActors.RemoveAt(IndexOfRemoval);

		OtherBoundCoreActor = BoundCoreActors[0];

	} else {
		BoundCoreActors.Empty();
	}
}

void ABoundCoreMainCharacter::MoveForward(float Value) {
	if ((Controller != NULL) && (Value != 0.0f)) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABoundCoreMainCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f)) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}
