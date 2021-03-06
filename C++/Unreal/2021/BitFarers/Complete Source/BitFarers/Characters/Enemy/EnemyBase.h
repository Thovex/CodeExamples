// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"

#include "BitFarers/Utilities/Interfaces/StatusInterface.h"
#include "BitFarers/Weapons/EElementalType.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnTakeDamage, EElementalType, ElementalType, float, Damage,
	const AActor*, DamageInstigator, const UActorComponent*, ImpactComponent, FVector, ImpactPoint);

UCLASS()
class BITFARERS_API AEnemyBase : public ACharacter, public IStatusInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase(const FObjectInitializer& ObjectInitializer);

	/* Delegates */
	
	UPROPERTY(BlueprintAssignable)
	FOnSpawn OnSpawnDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnDoAttack OnAttackDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnTakeDamage OnDamageDelegate;

	/* Properties */

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName EnemyName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStatusComponent* StatusComponent;

	UFUNCTION(BlueprintCallable)
	void Attack();
	
	

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Unreliable, NetMulticast)
	void NotifyDamage(EElementalType ElementalType, float Damage, const AActor* DamageInstigator,
                                 const UActorComponent* ImpactComponent, FVector ImpactPoint);

public:
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual float TakeDamage_Implementation(EElementalType ElementalType, float Damage, const AActor* DamageInstigator,
		const UActorComponent* ImpactComponent, FVector ImpactPoint) override;

	virtual bool IsPlayer_Implementation() override;



};
