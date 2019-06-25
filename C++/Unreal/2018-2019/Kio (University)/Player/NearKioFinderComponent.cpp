// Copyright 2018-2019 Prickly, All Rights Reserved.

#include "NearKioFinderComponent.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Core/Public/Math/Vector4.h"
#include "Player/KioCharacter.h"


UNearKioFinderComponent::UNearKioFinderComponent() {
	PrimaryComponentTick.bCanEverTick = true;

	KioCharacter = Cast<AKioCharacter>( GetOwner() );

	if ( KioCharacter != nullptr ) {
		KioCollisionComponent = KioCharacter->NearKioFinderCollision;
		CloseActorMapTypes.Add( FCloseActorMapTypes( FCloseActorDefinedProperties( "UseAble", 350, 0, 50, 0, ECloseActorStartValue::PlayerMesh ) ) );
		CloseActorMapTypes.Add( FCloseActorMapTypes( FCloseActorDefinedProperties( "DisplayAble", 650, 0, 90, 0, ECloseActorStartValue::PlayerCamera ), FColor::Blue ) );
	}
}

bool UNearKioFinderComponent::ReturnCloseActorMapByName( FName Name, FCloseActorMapTypes & ReturnData ) {
	for ( uint8 Index = 0; Index < CloseActorMapTypes.Num(); Index++ ) {
		if ( CloseActorMapTypes[Index].MapProperties.GetMapName() == Name ) {
			ReturnData = CloseActorMapTypes[Index];
			return true;
		}
	}
	return false;
}


void UNearKioFinderComponent::BeginPlay() {
	Super::BeginPlay();

	CreateDirectionVectors();

	KioCharacter = Cast<AKioCharacter>( GetOwner() );

	if ( !KioCollisionComponent ) {
		DestroyComponent();
	}

	KioCollisionComponent->OnComponentBeginOverlap.AddDynamic( this, &UNearKioFinderComponent::BeginOverlap );
	KioCollisionComponent->OnComponentEndOverlap.AddDynamic( this, &UNearKioFinderComponent::EndOverlap );

	KIO_LOG( FString( "Bound BeginOverlap & EndOverlap in NearKioFinderComponent.cpp" ) );

	KioCollisionComponent->SetSphereRadius( SphereRadius, true );
}

void UNearKioFinderComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) {
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	RETURN_IF_NULLPTR( KioCharacter );

	for ( uint8 AIndex = 0; AIndex < CloseActors.Num(); AIndex++ ) {
		for ( uint8 BIndex = 0; BIndex < CloseActorMapTypes.Num(); BIndex++ ) {

			FCloseActor Definitions = FCloseActor( 0, 0, FVector::ZeroVector, CloseActors[AIndex] );

			if ( DoNearKioCalculations( CloseActorMapTypes[BIndex].MapProperties, Definitions ) ) {
				CloseActorMapTypes[BIndex].CloseActorMaps.Add( CloseActors[AIndex], Definitions );
			} else {
				CloseActorMapTypes[BIndex].CloseActorMaps.Remove( CloseActors[AIndex] );
			}
		}
	}

	if ( bDebugOn ) {

		for ( uint8 AIndex = 0; AIndex < CloseActorMapTypes.Num(); AIndex++ ) {
			TArray<FCloseActor> Values;
			CloseActorMapTypes[AIndex].CloseActorMaps.GenerateValueArray( Values );

			const UWorld* World = GetWorld();

			for ( uint8 BIndex = 0; BIndex < Values.Num(); BIndex++ ) {
				FVector AddValue = FVector( 0, 0, ( AIndex + 1 ) * 10 );

				FVector ZOriginLocation = OriginLocation + AddValue;
				FVector ZValueLocation = Values[BIndex].Location + AddValue;;

				DrawDebugLine( World, ZOriginLocation, Values[BIndex].Location, CloseActorMapTypes[AIndex].DebugColor, false, DeltaTime, -1, 1.F );
				DrawDebugPoint( World, ZOriginLocation, 10.F, CloseActorMapTypes[AIndex].DebugColor, false, DeltaTime, -1 );
				DrawDebugPoint( World, Values[BIndex].Location, 10.F, CloseActorMapTypes[AIndex].DebugColor, false, DeltaTime, -1 );


				FString LeftStr, RightStr;

				FString AngleString = FString::SanitizeFloat( Values[BIndex].Angle );
				AngleString.Split( ".", &LeftStr, &RightStr );

				DrawDebugString( World, ( ( ZOriginLocation + Values[BIndex].Location ) / 2 ), FString::Printf( TEXT( "\370 %s" ), *LeftStr ), nullptr, CloseActorMapTypes[AIndex].DebugColor, DeltaTime, false, 1.F );


			}
		}
	}
}

void UNearKioFinderComponent::CreateDirectionVectors() {
	for ( int8 Indices = 0; Indices <= 36; Indices++ ) {
		float IndiceVal = float( Indices ) * 10.F;
		DirectionVectors.Add( FRotator( 0, IndiceVal, 0 ).RotateVector( FVector::ForwardVector ) );
	}
}

FVector UNearKioFinderComponent::ClosestDirection( FVector InVector ) {
	float MaxDot = MIN_FLOAT;
	FVector Ret = FVector::ZeroVector;

	for ( FVector Dir : DirectionVectors ) {
		float TempDot = FVector::DotProduct( InVector, Dir );
		if ( TempDot > MaxDot ) {
			Ret = Dir;
			MaxDot = TempDot;
		}
	}
	return Ret;
}

bool UNearKioFinderComponent::DoNearKioCalculations( FCloseActorDefinedProperties Properties, FCloseActor& CloseActorValues ) {

	switch ( Properties.GetStartValue() ) {
		case ECloseActorStartValue::PlayerMesh:
		{
			OriginLocation = KioCharacter->GetActorLocation();
			OriginForward = KioCharacter->GetActorForwardVector();
		} break;
		case ECloseActorStartValue::PlayerCamera:
		{
			OriginLocation = KioCharacter->GetActorLocation();
			OriginForward = ClosestDirection( KioCharacter->GetFollowCamera()->GetForwardVector() );
		} break;
		default:
		{
			OriginLocation = KioCharacter->GetActorLocation();
			OriginForward = KioCharacter->GetActorForwardVector();
		} break;

	}

	FVector TargetLocation = CloseActorValues.TargetActor->GetActorLocation();
	TargetLocation.Z = OriginLocation.Z;

	float Distance = CalculateDistance( OriginLocation, TargetLocation );

	if ( Distance < Properties.GetMinDistance() || Distance > Properties.GetMaxDistance() ) {
		return false;
	}

	float AngleDegrees = CalculateAngleDegrees( OriginLocation, OriginForward, TargetLocation );
	if ( AngleDegrees < Properties.GetMinAngle() || AngleDegrees > Properties.GetMaxAngle() ) {
		return false;
	}

	CloseActorValues.SetDistance( Distance );
	CloseActorValues.SetAngle( AngleDegrees );
	CloseActorValues.SetLocation( TargetLocation );

	return true;
}

float UNearKioFinderComponent::CalculateDistance( const FVector Origin, const FVector Target ) {
	return ( Origin - Target ).Size();
}

float UNearKioFinderComponent::CalculateAngleDegrees( const FVector Origin, const FVector OriginForwardVector, const FVector Target ) {
	return FMath::RadiansToDegrees( FMath::Acos( FVector::DotProduct( ( Target - Origin ).GetSafeNormal(), OriginForwardVector ) ) );
}

void UNearKioFinderComponent::AddToMaps( AActor* OtherActor ) {
	CloseActors.AddUnique( OtherActor );
}

void UNearKioFinderComponent::RemoveFromMaps( AActor* OtherActor ) {
	CloseActors.Remove( OtherActor );

	for ( uint8 AIndex = 0; AIndex < CloseActorMapTypes.Num(); AIndex++ ) {
		CloseActorMapTypes[AIndex].CloseActorMaps.Remove( OtherActor );
	}
}

void UNearKioFinderComponent::BeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult ) {
	AddToMaps( OtherActor );
}

void UNearKioFinderComponent::EndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex ) {
	RemoveFromMaps( OtherActor );
}

