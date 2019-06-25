// Copyright 2018 StrateGis Groep B.V. All Rights Reserved.
// Class AActorPawn created by Jesse van Vliet

#include "ActorPawn.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Framework/FrameworkInstance.h"
#include "Framework/Players/ActorPlayerController.h"
#include "UnrealNetwork.h"
#include "Engine/Engine.h"


AActorPawn::AActorPawn( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	PrimaryActorTick.bCanEverTick = true;

	TransformSceneComponent = CreateDefaultSubobject<USceneComponent>( FName( "TransformSceneComponent" ) );

	if ( TransformSceneComponent && RootComponent )
	{
		TransformSceneComponent->SetupAttachment( RootComponent );
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>( FName( "SpringArm" ) );

	if ( SpringArm )
	{
		SpringArm->SetupAttachment( TransformSceneComponent );
	}

	bReplicates = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>( FName( "CameraComponent" ) );

	if ( CameraComponent )
	{
		CameraComponent->SetupAttachment( SpringArm );
	}

	bReplicates = true;

}

void AActorPawn::SetLerpPositionRotation( FVector NewLerpPosition, FRotator NewLerpRotation )
{
	LerpPosition = NewLerpPosition;
	LerpRotation = NewLerpRotation;
}

void AActorPawn::SetLerpPositionRotation( FVector NewLerpPosition, FQuat NewLerpRotation )
{
	LerpPosition = NewLerpPosition;
	LerpRotation = FRotator( NewLerpRotation );
}

void AActorPawn::BeginPlay()
{
	Super::BeginPlay();

	GetRequiredReferences();

	InitialTransform = CameraComponent->GetComponentTransform();

	SetLerpPositionRotation( InitialTransform.GetLocation(), InitialTransform.GetRotation() );
}

void AActorPawn::GetRequiredReferences()
{
	const UWorld* World = GetWorld();

	if ( World )
	{
		if ( UGameplayStatics::GetPlayerController( World, 0 ) )
		{
			PlayerActorController = Cast<AActorPlayerController>( UGameplayStatics::GetPlayerController( World, 0 ) );
		}

		if ( World->GetGameState() )
		{
			FrameworkGameState = Cast<AFrameworkGameState>( World->GetGameState() );
		}

		FrameworkInstance = Cast<UFrameworkInstance>( GetGameInstance() );
	}
}

void AActorPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	/*
	if ( LerpPosition.Z < 1000 )
	{
		LerpPosition.Z = 1000;
	}

	if ( LerpPosition.Z > 15000 )
	{
		LerpPosition.Z = 15000;
	}

	HandleInput();
	HandleCamera();
	*/
}

void AActorPawn::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	check( InputComponent );

	InputComponent->BindKey( EKeys::W, IE_Pressed, this, &AActorPawn::MoveUpPressed );
	InputComponent->BindKey( EKeys::W, IE_Released, this, &AActorPawn::MoveUpReleased );

	InputComponent->BindKey( EKeys::A, IE_Pressed, this, &AActorPawn::MoveLeftPressed );
	InputComponent->BindKey( EKeys::A, IE_Released, this, &AActorPawn::MoveLeftReleased );

	InputComponent->BindKey( EKeys::S, IE_Pressed, this, &AActorPawn::MoveDownPressed );
	InputComponent->BindKey( EKeys::S, IE_Released, this, &AActorPawn::MoveDownReleased );

	InputComponent->BindKey( EKeys::D, IE_Pressed, this, &AActorPawn::MoveRightPressed );
	InputComponent->BindKey( EKeys::D, IE_Released, this, &AActorPawn::MoveRightReleased );

	InputComponent->BindKey( EKeys::MouseScrollUp, IE_Pressed, this, &AActorPawn::ScrollUp );
	InputComponent->BindKey( EKeys::MouseScrollDown, IE_Pressed, this, &AActorPawn::ScrollDown );

	InputComponent->BindKey( EKeys::LeftShift, IE_Pressed, this, &AActorPawn::SpeedActionPressed );
	InputComponent->BindKey( EKeys::LeftShift, IE_Released, this, &AActorPawn::SpeedActionReleased );

	InputComponent->BindKey( EKeys::RightMouseButton, IE_Pressed, this, &AActorPawn::CommandRMB );

	InputComponent->BindKey( EKeys::SpaceBar, IE_Pressed, this, &AActorPawn::EndTurn );
}

void AActorPawn::SetCenterInitial()
{
	CenterActor->SetNewLocation( CenterActor->InitialPosition );
}

void AActorPawn::CommandRMB()
{
	PlayerActorController->SetLocationCameraHold( nullptr );
	SetCenterInitial();
	PlayerActorController->SelectedPlot = nullptr;
}

void AActorPawn::ResetCamera()
{
	SetCenterInitial();
	LerpPosition = InitialTransform.GetLocation();
	LerpRotation = FRotator( InitialTransform.GetRotation() );
}

void AActorPawn::ServerInitializePawn_Implementation( AActorPlayerController* PlayerAController )
{
	ActorCapital = FrameworkGameState->ReturnActorsByString( PlayerAController->PlayerActor ).StartCapital;
}

void AActorPawn::ServerSetCapitalAdd_Implementation( float CapitalToAdd )
{

	ActorCapital = ActorCapital + CapitalToAdd;
}

void AActorPawn::ServerSetCapitalSub_Implementation( float CapitalToSub )
{
	
	ActorCapital = ActorCapital - CapitalToSub;
}

void AActorPawn::RotateCamera( float RotationSpeed )
{

	const UWorld* World = GetWorld();

	if ( World )
	{
		if ( bHoldingShift )
		{
			RotationSpeed *= 3.0f;
		}

		if ( CameraComponent && CenterActor )
		{
			FVector OrbitDirection = CameraComponent->GetComponentLocation() - CenterActor->GetActorLocation();

			float AngleDegree = World->GetDeltaSeconds() * RotationSpeed;
			FVector RotationAxis = FVector( 0, 0, 1 );

			FVector RotatedVector = UKismetMathLibrary::RotateAngleAxis( OrbitDirection, AngleDegree, RotationAxis );

			RotatedVector -= OrbitDirection;
			LerpPosition += RotatedVector;
		}
	}


}

void AActorPawn::ZoomCamera( float ZoomSpeed )
{
	if ( bHoldingShift )
	{
		ZoomSpeed *= 3.0f;
	}

	LerpPosition += CameraComponent->GetForwardVector() * ZoomSpeed;
}

void AActorPawn::HandleInput()
{
	if ( MoveKeysPressed.Contains( true ) )
	{
		for ( uint8 i = 0; i < MoveKeysPressed.Num(); i++ )
		{
			if ( MoveKeysPressed[i] )
			{
				switch ( i )
				{
					// Input W
					case 0:
					{
						if ( FVector::Dist( CenterActor->GetActorLocation(), CameraComponent->GetComponentLocation() ) > 3750 )
						{
							ZoomCamera( 75.0F );
						}
					} break;
					// Input A
					case 1:
					{
						RotateCamera( 35.0F );
					} break;
					// Input S
					case 2:
					{
						ZoomCamera( -75.0F );
					} break;
					// Input D
					case 3:
					{
						RotateCamera( -35.0F );
					} break;
				}
			}
		}
	}
}

void AActorPawn::EndTurn()
{
	if ( PlayerActorController )
	{
		if ( FrameworkGameState )
		{
			if ( !FrameworkGameState->bIsTurnBasedOn ) return;
		}

		if ( PlayerActorController->bIsTurn )
		{
			PlayerActorController->ServerEndTurn();
		}
	}
}

void AActorPawn::ScrollUp()
{
	LerpPosition.Z += 1000;
}

void AActorPawn::ScrollDown()
{
	if ( FVector::Dist( CenterActor->GetActorLocation(), CameraComponent->GetComponentLocation() ) > 3750 )
	{
		LerpPosition.Z -= 1000;
	}
}

void AActorPawn::HandleCamera()
{
	const UWorld* World = GetWorld();

	if ( World )
	{
		if ( CameraComponent && CenterActor )
		{
			FVector LerpedPosition = FMath::Lerp( CameraComponent->GetComponentLocation(), LerpPosition, 0.1F );
			FQuat LerpedRotation = FQuat::Slerp( FQuat( CameraComponent->GetComponentRotation() ), FQuat( LerpRotation ), 0.1F );

			CameraComponent->SetWorldLocationAndRotation( LerpedPosition, LerpedRotation, false, nullptr, ETeleportType::None );

			FRotator CameraRotation = UKismetMathLibrary::FindLookAtRotation( CameraComponent->GetComponentLocation(), CenterActor->GetActorLocation() );

			CameraComponent->SetWorldRotation( CameraRotation );
		}
	}

}


void AActorPawn::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{

	Super::GetLifetimeReplicatedProps( OutLifetimeProps );
	DOREPLIFETIME( AActorPawn, ActorCapital );

}
