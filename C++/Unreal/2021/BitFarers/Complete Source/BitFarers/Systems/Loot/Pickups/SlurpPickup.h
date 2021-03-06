// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"

#include "BitFarers/Weapons/Ammo/EAmmoType.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "SlurpPickup.generated.h"

class APlayerCharacter;

UCLASS(Abstract)
class BITFARERS_API ASlurpPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	ASlurpPickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	USkeletalMeshComponent* PickupMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	USphereComponent* PickupRadius;

	float DistanceToPickup = 50;
	float DistanceToScale = 500;

	bool bPickupAble = false;

	UFUNCTION(BlueprintCallable)
	void StartInterpolation();

	UFUNCTION(BlueprintCallable)
	void SetRotationToMaterials(FVector Rotation, float Speed);
	

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	virtual bool Prerequisites();
	virtual void ExecutePickup();

	void ClearIdle();
	void InitializeMaterials();
	void HandleIdleTime();

	static EAmmoType ClassifyAmmoType(FString ItemName);

	float IdleTime = 0;
	
	bool bOnPickup = false;
	bool bIsPickingUp = false;
	bool bIsPickedUp = false;

	UFUNCTION(Server, Reliable)
	void DestroyAuthority();
	
	UPROPERTY()
	APlayerCharacter* InterpolationTarget;

	UFUNCTION()
         virtual void OnPickupOverlapBegin(
         	class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
         	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
        virtual void OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	FTimerHandle InterpolationTimer;
	
	void Interpolator();

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> PickupMeshMaterials;

};