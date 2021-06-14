// Copyright Baer and Hoggo Games


#include "WeaponShootUtilityLibrary.h"


#include "WeaponStats.h"
#include "BitFarers/Macros.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

void UWeaponShootUtilityLibrary::GetUnitVectors(
    USceneComponent* Component, FVector& Up, FVector& Right, FVector& Forward)
{
	
	RETURN_IF_NULLPTR(Component);
	
	Up = Component->GetUpVector();
	Right = Component->GetRightVector();
	Forward = Component->GetForwardVector();
}

void UWeaponShootUtilityLibrary::GenerateShotRaycastsByStreamCreation(const UObject* WorldContextObject, const float Range,
	const float HitSpread, const FVector Location, const ACharacter* CharacterRef, const int32 RandomSeed, const int32 Count,
	TArray<FVector>& StartPoints, TArray<FVector>& EndPoints)
{
	const UWorld* World = WorldContextObject->GetWorld();
	RETURN_IF_NULLPTR(World);

	FVector CamUpVector, CamRightVector, CamForwardVector;
	
	USceneComponent* Camera = Cast<USceneComponent>(CharacterRef->GetComponentByClass(UCameraComponent::StaticClass()));
	USceneComponent* Capsule = Cast<USceneComponent>(CharacterRef->GetCapsuleComponent());

	GetUnitVectors(Camera ? Camera : Capsule, CamUpVector, CamRightVector, CamForwardVector);


	const FRandomStream Rnd = FRandomStream(RandomSeed);

	FCollisionQueryParams Params = FCollisionQueryParams();
	Params.AddIgnoredActor(CharacterRef);
	Params.bTraceComplex = true;

	for (int32 i = 0; i < Count; i++)
	{
		const FVector ShootingDirection =
        (CamForwardVector +
            CamRightVector * Rnd.FRandRange(-HitSpread, HitSpread) +
            CamUpVector * Rnd.FRandRange(-HitSpread, HitSpread))
        * Range;

		const FVector EndLocation = Location + ShootingDirection;

		FHitResult HitResult;
		World->LineTraceSingleByChannel(
            HitResult, Location, EndLocation, ECC_GameTraceChannel2, Params);

		StartPoints.Add(HitResult.TraceStart);
		EndPoints.Add(HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd);
	}
}

void UWeaponShootUtilityLibrary::GenerateShotRaycasts(const UObject* WorldContextObject, ACharacter* CharacterRef, FWeaponStats WeaponStats, USceneComponent* ShootDirection,
                                                      TArray<FVector>& StartPoints, TArray<FVector>& EndPoints)
{
	const UWorld* World = WorldContextObject->GetWorld();
	RETURN_IF_NULLPTR(World);

	FVector CamUpVector, CamRightVector, CamForwardVector;
	
	USceneComponent* Camera = Cast<USceneComponent>(CharacterRef->GetComponentByClass(UCameraComponent::StaticClass()));
	USceneComponent* Capsule = Cast<USceneComponent>(CharacterRef->GetCapsuleComponent());

	GetUnitVectors(Camera ? Camera : Capsule, CamUpVector, CamRightVector, CamForwardVector);

	const FVector ShootingLocation = ShootDirection->GetComponentLocation();

	FCollisionQueryParams Params = FCollisionQueryParams();
	Params.bTraceComplex = true;
	Params.AddIgnoredActor(CharacterRef);

	for (int32 i = 0; i < WeaponStats.CalculateDynamicProjectileCount(); i++)
	{
		const FVector ShootingDirection =
		(CamForwardVector +
			CamRightVector * FMath::RandRange(-WeaponStats.CalculateDynamicHitSpread(), WeaponStats.CalculateDynamicHitSpread()) +
			CamUpVector * FMath::RandRange(-WeaponStats.CalculateDynamicHitSpread(), WeaponStats.CalculateDynamicHitSpread()))
		* WeaponStats.CalculateDynamicRange();

		const FVector EndLocation = ShootingLocation + ShootingDirection;

		FHitResult HitResult;
		World->LineTraceSingleByChannel(
			HitResult, ShootingLocation, EndLocation, ECC_GameTraceChannel2, Params);

		StartPoints.Add(HitResult.TraceStart);
		EndPoints.Add(HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd);
	}
}

void UWeaponShootUtilityLibrary::GenerateShotRaycastsWithOutput(const UObject* WorldContextObject, ACharacter* CharacterRef, FWeaponStats WeaponStats,
	USceneComponent* ShootDirection, TArray<FHitResult>& Results)
{
	const UWorld* World = WorldContextObject->GetWorld();
	RETURN_IF_NULLPTR(World);

	FVector CamUpVector, CamRightVector, CamForwardVector;
	
	USceneComponent* Camera = Cast<USceneComponent>(CharacterRef->GetComponentByClass(UCameraComponent::StaticClass()));
	USceneComponent* Capsule = Cast<USceneComponent>(CharacterRef->GetCapsuleComponent());
	
	GetUnitVectors(Camera ? Camera : Capsule, CamUpVector, CamRightVector, CamForwardVector);

	const FVector ShootingLocation = ShootDirection->GetComponentLocation();

	FCollisionQueryParams Params = FCollisionQueryParams();
	Params.bTraceComplex = true;
	Params.AddIgnoredActor(CharacterRef);
	
	for (int32 i = 0; i < WeaponStats.CalculateDynamicProjectileCount(); i++)
	{
		const FVector ShootingDirection =
		(CamForwardVector +
			CamRightVector * FMath::RandRange(-WeaponStats.CalculateDynamicHitSpread(), WeaponStats.CalculateDynamicHitSpread()) +
			CamUpVector * FMath::RandRange(-WeaponStats.CalculateDynamicHitSpread(), WeaponStats.CalculateDynamicHitSpread()))
		* WeaponStats.CalculateDynamicRange();

		const FVector EndLocation = ShootingLocation + ShootingDirection;

		FHitResult HitResult;
		World->LineTraceSingleByChannel(
			HitResult, ShootingLocation, EndLocation, ECC_GameTraceChannel2, Params);

		Results.Add(HitResult);
	}
}