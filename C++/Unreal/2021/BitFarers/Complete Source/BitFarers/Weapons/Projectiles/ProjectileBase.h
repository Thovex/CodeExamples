// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"


#include "BitFarers/Weapons/EElementalType.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SmoothSyncPlugin/Public/SmoothSync.h"

#include "ProjectileBase.generated.h"

UCLASS()
class BITFARERS_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite) USphereComponent* ProjectileCollision;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) USphereComponent* ExplosionCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) class USmoothSync* SmoothSync;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true)) float Damage;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true)) EElementalType DamageType;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true)) bool IsPlayerInstigator;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const override;
private:
	FTimerHandle StartSpawnAnimationHandle;
	void StartSpawnAnimation();
};

