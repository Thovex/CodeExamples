// Copyright Baer and Hoggo Games


#include "ProjectileBase.h"

#include "Kismet/KismetMathLibrary.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	RootComponent = ProjectileCollision;

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->SetupAttachment(ProjectileCollision);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(ProjectileCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	SmoothSync = CreateDefaultSubobject<USmoothSync>(TEXT("SmoothSync"));
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* World = GetWorld();

	if (World)
	{
		ProjectileMesh->SetWorldScale3D(FVector::ZeroVector);

		World->GetTimerManager().SetTimer(StartSpawnAnimationHandle, this,
			&AProjectileBase::StartSpawnAnimation, World->GetDeltaSeconds(), true, -1);
	}
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::StartSpawnAnimation()
{
	const UWorld* World = GetWorld();

	const FVector NewScale =
		UKismetMathLibrary::VInterpTo(ProjectileMesh->GetComponentScale(), FVector::OneVector, World->GetDeltaSeconds(), 5);

	ProjectileMesh->SetWorldScale3D(NewScale);

	if (NewScale == FVector::OneVector)
	{
		World->GetTimerManager().ClearTimer(StartSpawnAnimationHandle);
	}
}

void AProjectileBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectileBase, Damage);
	DOREPLIFETIME(AProjectileBase, DamageType);
	DOREPLIFETIME(AProjectileBase, IsPlayerInstigator);
	
}

