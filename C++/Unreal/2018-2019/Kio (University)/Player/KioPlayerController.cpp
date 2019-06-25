// Copyright 2018-2019 Prickly, All Rights Reserved.

#include "KioPlayerController.h"
#include "Components//NearKioFinderComponent.h"
#include "DrawDebugHelpers.h"
#include "Interfaces/Useable.h"

AKioPlayerController::AKioPlayerController() {


}

void AKioPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAction( "Jump", IE_Pressed, this, &AKioPlayerController::Jump );
	InputComponent->BindAction( "Jump", IE_Released, this, &AKioPlayerController::StopJumping );

	InputComponent->BindAction( "Use", IE_Pressed, this, &AKioPlayerController::Use );

	InputComponent->BindAxis( "MoveForward", this, &AKioPlayerController::MoveForward );
	InputComponent->BindAxis( "MoveRight", this, &AKioPlayerController::MoveRight );

	InputComponent->BindAxis( "Turn", this, &AKioPlayerController::AddControllerYawInput );
	InputComponent->BindAxis( "TurnRate", this, &AKioPlayerController::TurnAtRate );

	InputComponent->BindAxis( "LookUp", this, &AKioPlayerController::AddControllerPitchInput );
	InputComponent->BindAxis( "LookUpRate", this, &AKioPlayerController::LookUpAtRate );

}

void AKioPlayerController::BeginPlay() {
	Super::BeginPlay();
}

void AKioPlayerController::Possess( APawn * InPawn ) {
	Super::Possess( InPawn );

	KioCharacter = Cast<AKioCharacter>( InPawn );

	KIO_LOG( FString( TEXT( "Possessed: " ) ).Append( InPawn->GetName() ) );
}

void AKioPlayerController::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
}

bool AKioPlayerController::SetInputState( EKioInputState NewInputState ) {
	KioInputState = NewInputState;

	if ( KioInputState != NewInputState ) {
		return false;
	}

	OnInputStateChanged.Broadcast( KioInputState );

	//KIO_LOG_EVENT_OneParam( "OnInputStateChanged", GetEnumValueAsString<EKioInputState>( "EKioInputState", KioInputState ) );

	return true;
}

void AKioPlayerController::MoveForward( float AxisValue ) {
	if ( AxisValue == 0.0f ) return;

	switch ( KioInputState ) {
		case EKioInputState::Kio_Default:
		{
			MoveForwardDefault( AxisValue );
		} break;

	}
}

void AKioPlayerController::MoveForwardDefault( float AxisValue ) {
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation( 0, Rotation.Yaw, 0 );

	const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );
	KioCharacter->AddMovementInput( Direction, AxisValue );
}


void AKioPlayerController::MoveRight( float AxisValue ) {
	if ( AxisValue == 0.0f ) return;

	switch ( KioInputState ) {
		case EKioInputState::Kio_Default:
		{
			MoveRightDefault( AxisValue );
		} break;
	}
}

void AKioPlayerController::MoveRightDefault( float AxisValue ) {
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation( 0, Rotation.Yaw, 0 );

	const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );
	KioCharacter->AddMovementInput( Direction, AxisValue );
}

void AKioPlayerController::Jump() {
	switch ( KioInputState ) {
		case EKioInputState::Kio_Default:
		{
			KioCharacter->Jump();
		} break;
}

void AKioPlayerController::StopJumping() {
	switch ( KioInputState ) {
		case EKioInputState::Kio_Default:
		{
			KioCharacter->StopJumping();
		} break;
	}
}

void AKioPlayerController::LookUpAtRate( float Rate ) {
	AddPitchInput( Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() );
}

void AKioPlayerController::TurnAtRate( float Rate ) {
	AddYawInput( Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() );
}

void AKioPlayerController::AddControllerPitchInput( float AxisValue ) {
	AddPitchInput( AxisValue );
}

void AKioPlayerController::AddControllerYawInput( float AxisValue ) {
	AddYawInput( AxisValue );
}

void AKioPlayerController::Use() {
	switch ( KioInputState ) {
		case EKioInputState::Kio_Default:
		{
			KioCharacter->UseSelectedActor();
		} break;
		case EKioInputState::Kio_Pillar:
		{
			KioCharacter->StopUsingSelectedActor();
		} break;

	}
}