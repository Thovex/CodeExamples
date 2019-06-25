// Copyright 2018-2019 Prickly, All Rights Reserved.

#include "MoveablePillar.h"
#include "Player/KioCharacter.h"
#include "Player/KioPlayerController.h"
#include "Framework/KioCustomBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"

AMoveablePillar::AMoveablePillar() {
	PrimaryActorTick.bCanEverTick = true;
}


void AMoveablePillar::BeginPlay() {
	Super::BeginPlay();

	KioCharacter = UKioCustomBlueprintLibrary::ReturnKioCharacter( GetWorld() );
	KioPlayerController = Cast<AKioPlayerController>( KioCharacter->GetController() );

	for ( auto& ToolTipComponent : SideTooltipsWithAlpha ) {
		ToolTipComponent.Key->CreateAndSetMaterialInstanceDynamic( 0 );
	}
}

void AMoveablePillar::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );

	if ( CurrentDisplayTime >= 0.1F ) {
		CurrentDisplayTime -= DeltaTime;
		OutlineSideSelector( DeltaTime );
	} else {
		ResetOutlineSides( DeltaTime );
	}

	if ( bIsMovingPlayer ) {
		InterpolateKioTowardsArrow( DeltaTime );
	}
}

void AMoveablePillar::InterpolateKioTowardsArrow( float DeltaTime ) {
	{
		KioCharacter->SetActorLocation( FMath::Lerp( KioCharacter->GetActorTransform().GetLocation(), PlayerGotoTransform.GetLocation(), DeltaTime * 10 ) );
		KioCharacter->SetActorRotation( FMath::Lerp( KioCharacter->GetActorTransform().GetRotation(), PlayerGotoTransform.GetRotation(), DeltaTime * 10 ) );

		if ( FVector::Dist( KioCharacter->GetActorLocation(), PlayerGotoTransform.GetLocation() ) < 35 ) {

			FVector KioEulerAngle = KioCharacter->GetActorRotation().Euler();
			FVector GotoEulerAngle = PlayerGotoTransform.GetRotation().Euler();

			float AngleDifference = KioEulerAngle.Z - GotoEulerAngle.Z;

			if ( AngleDifference < 2.F ) {
				EndInterpolateKioTowardsArrow();
			}
		}
	}
}

void AMoveablePillar::Outline_Implementation( bool DisplayValue ) {
	CurrentDisplayTime = DisplayValue ? 0.2F : 0.0F;
}

void AMoveablePillar::ResetOutlineSides( float DeltaTime ) {
	float HighestAlpha = MIN_FLOAT;

	if ( !bHasResetOutlineSides ) {
		for ( auto& ToolTipComponent : SideTooltipsWithAlpha ) {
			if ( ToolTipComponent.Value > 0.F ) {
				ToolTipComponent.Value -= DeltaTime * 2.F;
			} 

			if ( ToolTipComponent.Value > HighestAlpha ) {
				HighestAlpha = ToolTipComponent.Value;
			}

			Cast< UMaterialInstanceDynamic>( ToolTipComponent.Key->GetMaterial( 0 ) )->SetScalarParameterValue( FName( "Alpha" ), ToolTipComponent.Value );
		}
	}

	if ( HighestAlpha <= 0.F ) {
		bHasResetOutlineSides = true;
	}
}

void AMoveablePillar::OutlineSideSelector( float DeltaTime ) {

	TArray<USceneComponent*> ClosestArrowChildren;
	UStaticMeshComponent* SelectedToolTipComponent = NULL;

	FindClosestArrowToPlayer()->GetChildrenComponents( true, ClosestArrowChildren );

	for ( USceneComponent* ClosestArrowChild : ClosestArrowChildren ) {
		if ( ClosestArrowChild->ComponentTags.Contains( "ToolTip" ) ) {
			SelectedToolTipComponent = Cast<UStaticMeshComponent>( ClosestArrowChild );
		}
	}

	if ( SelectedToolTipComponent ) {
		for ( auto& ToolTipComponent : SideTooltipsWithAlpha ) {
			if ( ToolTipComponent.Key == SelectedToolTipComponent ) {
				if ( ToolTipComponent.Value <= 1.F ) {
					ToolTipComponent.Value += DeltaTime * 2.F;
				}
			} else {
				if ( ToolTipComponent.Value > 0.F ) {
					ToolTipComponent.Value -= DeltaTime * 2.F;
				}
			}
			Cast< UMaterialInstanceDynamic>( ToolTipComponent.Key->GetMaterial( 0 ) )->SetScalarParameterValue( FName( "Alpha" ), ToolTipComponent.Value );
		}
	}

	bHasResetOutlineSides = false;

}

void AMoveablePillar::Use_Implementation() {
	bIsActive = true;
	StartInterpolateKioTowardsArrow( FindClosestArrowToPlayer() );
}

void AMoveablePillar::StopUsing_Implementation() {
	bIsActive = false;
	KioPlayerController->SetInputState( EKioInputState::Kio_Default );
}

void AMoveablePillar::StartInterpolateKioTowardsArrow( UArrowComponent* ArrowToInterpolateTowards ) {
	RETURN_IF_NULLPTR( ArrowToInterpolateTowards );

	if ( KioPlayerController->SetInputState( EKioInputState::Kio_Disabled ) ) {
		PlayerGotoTransform = ArrowToInterpolateTowards->GetComponentTransform();
		bIsMovingPlayer = true;
	}
}

void AMoveablePillar::EndInterpolateKioTowardsArrow() {
	KioCharacter->SetActorLocation( PlayerGotoTransform.GetLocation() );
	KioCharacter->SetActorRotation( PlayerGotoTransform.GetRotation() );

	if ( KioPlayerController->SetInputState( EKioInputState::Kio_Pillar ) ) {
		bIsMovingPlayer = false;
	}
}

UArrowComponent* AMoveablePillar::FindClosestArrowToPlayer() {
	float ClosestDistance = MAX_FLOAT;
	UArrowComponent* ClosestArrow = NULL;

	for ( auto& SideCollision : SideCollisions ) {
		if ( !DisabledDirections.Contains( SideCollision.Key ) ) {
			UArrowComponent* SideCollisionArrowParent = Cast<UArrowComponent>( SideCollision.Value->GetAttachParent() );
			float DistanceKioToCollision = FVector::Dist( SideCollisionArrowParent->GetComponentLocation(), KioCharacter->GetMesh()->GetComponentLocation() );

			if ( DistanceKioToCollision < ClosestDistance ) {
				ClosestDistance = DistanceKioToCollision;
				ClosestArrow = SideCollisionArrowParent;
			}
		}
	}

	return ClosestArrow;
}
