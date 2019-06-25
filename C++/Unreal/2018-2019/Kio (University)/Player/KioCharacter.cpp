// Copyright 2018-2019 Prickly, All Rights Reserved.

#include "KioCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/NearKioFinderComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Useable.h"

AKioCharacter::AKioCharacter() {
	GetCapsuleComponent()->InitCapsuleSize( 42.f, 96.0f );

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator( 0.0f, 540.0f, 0.0f );
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>( TEXT( "CameraBoom" ) );
	CameraBoom->SetupAttachment( RootComponent );
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	NearKioFinderCollision = CreateDefaultSubobject<USphereComponent>( TEXT( "NearKioFinderCollision" ) );
	NearKioFinderCollision->SetupAttachment( RootComponent );

	NearKioFinderComponent = CreateDefaultSubobject<UNearKioFinderComponent>( TEXT( "NearKioFinderComponent" ) );

	FollowCamera = CreateDefaultSubobject<UCameraComponent>( TEXT( "FollowCamera" ) );
	FollowCamera->SetupAttachment( CameraBoom, USpringArmComponent::SocketName );
	FollowCamera->bUsePawnControlRotation = false;
}

void AKioCharacter::SetCurrentSelectedActor( AActor* ActorToSelect ) {
	if ( ActorToSelect ) {
		if ( ActorToSelect->GetClass()->ImplementsInterface( UUseable::StaticClass() ) ) {
			CurrentSelectedActor = ActorToSelect;
		}
	} else {
		CurrentSelectedActor = NULL;
	}
}

void AKioCharacter::UseSelectedActor() {
	if ( CurrentSelectedActor ) {
		IUseable::Execute_Use( CurrentSelectedActor );
	}
}

void AKioCharacter::StopUsingSelectedActor() {
	if ( CurrentSelectedActor ) {
		IUseable::Execute_StopUsing( CurrentSelectedActor );
	}
}

void AKioCharacter::BeginPlay() {
	Super::BeginPlay();
}
