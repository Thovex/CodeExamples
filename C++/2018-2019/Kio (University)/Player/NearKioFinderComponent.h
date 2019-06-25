// Copyright 2018-2019 Prickly, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "NearKioFinderComponent.generated.h"

class AKioCharacter;

UENUM( BlueprintType )
enum class ECloseActorStartValue : uint8 {
	PlayerMesh                     UMETA( DisplayName = "PlayerMesh" ),
	PlayerCamera                   UMETA( DisplayName = "PlayerCamera" )
};

USTRUCT( BlueprintType )
struct FCloseActor {
	GENERATED_USTRUCT_BODY()

		UPROPERTY( EditAnywhere, BlueprintReadWrite ) float Distance;
	UPROPERTY( EditAnywhere, BlueprintReadWrite ) float Angle;
	UPROPERTY( EditAnywhere, BlueprintReadWrite ) FVector Location;
	UPROPERTY( EditAnywhere, BlueprintReadWrite ) AActor* TargetActor;

	FCloseActor() {
		Distance = 0.F;
		Angle = 0.F;
		Location = FVector::ZeroVector;
		TargetActor = NULL;
	}

	FCloseActor( float _Distance, float _Angle, FVector _Location, AActor* _TargetActor ) {
		Distance = _Distance;
		Angle = _Angle;
		Location = _Location;
		TargetActor = _TargetActor;
	}

	void SetDistance( const float NewDistance ) {
		Distance = NewDistance;
	}

	void SetAngle( const float NewAngle ) {
		Angle = NewAngle;
	}

	void SetLocation( const FVector NewLocation ) {
		Location = NewLocation;
	}

	void SetActor( AActor* NewActor ) {
		TargetActor = NewActor;
	}

	bool ActorIsValid() const {
		if ( !TargetActor ) return false;
		return TargetActor->IsValidLowLevel();
	}

	bool CheckValidDistance( const float MaxDistance ) const {
		if ( Distance < MaxDistance ) return true;
		return false;
	}

	bool CheckValidAngle( const float MaxAngle ) const {
		if ( Angle < MaxAngle ) return true;
		return false;
	}

	bool CheckValid( const float MaxDistance, const float MaxAngle ) const {
		if ( !ActorIsValid() ) return false;
		if ( !CheckValidDistance( MaxDistance ) ) return false;
		if ( !CheckValidAngle( MaxAngle ) ) return false;
		return true;
	}
};

struct FSortCloseActorByDistance {
	bool operator()( const FCloseActor& A, const FCloseActor& B ) const {
		if ( A.Distance < B.Distance ) {
			return true;
		}
		return false;
	}
};

struct FSortCloseActorByAngle {
	bool operator() ( const FCloseActor& A, const FCloseActor& B ) const {
		if ( A.Angle < B.Angle ) {
			return true;
		}
		return false;
	}
};

USTRUCT( BlueprintType )
struct FCloseActorDefinedProperties {
	GENERATED_USTRUCT_BODY()

		UPROPERTY( EditAnywhere, BlueprintReadWrite ) FName MapName;

	UPROPERTY( EditAnywhere, BlueprintReadWrite ) float MaxDistance;
	UPROPERTY( EditAnywhere, BlueprintReadWrite ) float MinDistance;

	UPROPERTY( EditAnywhere, BlueprintReadWrite ) float MaxAngle;
	UPROPERTY( EditAnywhere, BlueprintReadWrite ) float MinAngle;

	UPROPERTY( EditAnywhere, BlueprintReadWrite ) ECloseActorStartValue StartValue;

	FCloseActorDefinedProperties() {
		MapName = FName( "New Map" );
		MaxDistance = 0.F;
		MinDistance = 0.F;
		MaxAngle = 0.F;
		MinAngle = 0.F;
		StartValue = ECloseActorStartValue::PlayerMesh;
	}

	FCloseActorDefinedProperties( FName _MapName, float _MaxDistance, float _MinDistance, float _MaxAngle, float _MinAngle, ECloseActorStartValue _StartValue ) {
		MapName = _MapName;
		MaxDistance = _MaxDistance;
		MinDistance = _MinDistance;
		MaxAngle = _MaxAngle;
		MinAngle = _MinAngle;
		StartValue = _StartValue;
	}

	float GetMaxDistance() { return MaxDistance; }
	float GetMinDistance() { return MinDistance; }

	float GetMaxAngle() { return MaxAngle; }
	float GetMinAngle() { return MinAngle; }

	FName GetMapName() { return MapName; }

	ECloseActorStartValue GetStartValue() { return StartValue; }
};


USTRUCT( BlueprintType )
struct FCloseActorMapTypes {
	GENERATED_USTRUCT_BODY()

		UPROPERTY( EditAnywhere, BlueprintReadWrite ) FCloseActorDefinedProperties MapProperties;
	UPROPERTY( EditAnywhere, BlueprintReadWrite ) TMap<AActor*, FCloseActor> CloseActorMaps;
	UPROPERTY( EditAnywhere, BlueprintReadWrite ) FColor DebugColor;

	FCloseActorMapTypes() {
		MapProperties = FCloseActorDefinedProperties();
		CloseActorMaps = TMap<AActor*, FCloseActor>();
		DebugColor = FColor::Red;
	}

	FCloseActorMapTypes( FCloseActorDefinedProperties _MapProperties, FColor _DebugColor = FColor::Red ) {
		MapProperties = _MapProperties;
		CloseActorMaps = TMap<AActor*, FCloseActor>();
		DebugColor = _DebugColor;
	}
};

UCLASS( ClassGroup = ( Kio ), meta = ( BlueprintSpawnableComponent ) )
class KIO_API UNearKioFinderComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UNearKioFinderComponent();

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Near Kio Component" )
		float SphereRadius = 512;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Near Kio Component" )
		bool bDebugOn = false;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Near Kio Component" )
		TArray< FCloseActorMapTypes > CloseActorMapTypes;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Near Kio Component" )
		TArray <FVector> DirectionVectors;


public:

	UFUNCTION( BlueprintCallable )
		bool ReturnCloseActorMapByName( FName Name, FCloseActorMapTypes& ReturnData );

protected:
	AKioCharacter* KioCharacter;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION()
		virtual void BeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult );

	UFUNCTION()
		virtual void EndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex );

private:
	USphereComponent* KioCollisionComponent;

	FVector OriginLocation;
	FVector OriginForward;

	TArray < AActor*> CloseActors;


private:
	float CalculateDistance( const FVector Origin, const FVector Target );
	float CalculateAngleDegrees( const FVector Origin, const FVector OriginForwardVector, const FVector Target );

	bool DoNearKioCalculations( FCloseActorDefinedProperties Properties, FCloseActor& CloseActorValues );

	void AddToMaps( AActor* OtherActor );
	void RemoveFromMaps( AActor* OtherActor );

	FVector ClosestDirection( FVector InVector );
	void CreateDirectionVectors();

};
