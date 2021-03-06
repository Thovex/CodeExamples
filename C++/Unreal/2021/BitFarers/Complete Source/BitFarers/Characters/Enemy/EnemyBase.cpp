// Copyright Baer and Hoggo Games


#include "EnemyBase.h"
#include "BitFarers/Systems/Movement/CustomCharacterMovement.h"
#include "BitFarers/Macros.h"

// Sets default values
AEnemyBase::AEnemyBase(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovement>(CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	INI_COMP(StatusComponent, UStatusComponent, "Status");
}

float AEnemyBase::TakeDamage_Implementation(const EElementalType ElementalType, const float Damage,
                                            const AActor* DamageInstigator, const UActorComponent* ImpactComponent,
                                            const FVector ImpactPoint)
{
	const float CalculatedDamage = StatusComponent->TakeDamage(ElementalType, Damage, DamageInstigator, ImpactComponent,
	                                                           ImpactPoint);
	NotifyDamage(ElementalType, CalculatedDamage, DamageInstigator, ImpactComponent, ImpactPoint);

	return CalculatedDamage;
}

void AEnemyBase::NotifyDamage_Implementation(const EElementalType ElementalType, const float Damage,
                                              const AActor* DamageInstigator, const UActorComponent* ImpactComponent,
                                              const FVector ImpactPoint)
{
	OnDamageDelegate.Broadcast(ElementalType, Damage, DamageInstigator, ImpactComponent, ImpactPoint);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	Execute_InitializeStatus(this);

	OnSpawnDelegate.Broadcast();
}

void AEnemyBase::Attack()
{
	OnAttackDelegate.Broadcast();
}

bool AEnemyBase::IsPlayer_Implementation()
{
	return false;
}
