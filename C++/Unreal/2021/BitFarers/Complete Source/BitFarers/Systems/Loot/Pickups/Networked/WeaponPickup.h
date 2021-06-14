// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"


#include "BitFarers/Utilities/Interfaces/ButtonClickable.h"
#include "BitFarers/Systems/Loot/Pickups/SlurpPickup.h"
#include "BitFarers/Weapons/WeaponStats.h"

#include "WeaponPickup.generated.h"

UCLASS()
class BITFARERS_API AWeaponPickup final : public ASlurpPickup, public IButtonClickable
{

	GENERATED_BODY()

public:
	AWeaponPickup();

	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_SetWeaponData, BlueprintReadWrite, Category="Pickup")
	TArray<FWeaponStats> WeaponData;

	UFUNCTION()
    void OnRep_SetWeaponData();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Pickup")
    void InitializeWeapon(const TArray<FWeaponStats>& InWeaponData);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Pickup")
    void InitializeWeapon_Response(const TArray<FWeaponStats>& InWeaponData);

protected:
	virtual void BeginPlay() override;
	virtual bool Prerequisites() override;
	virtual void ExecutePickup() override;

	virtual void OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const override;

public:
	virtual void DoButtonClick_Implementation(int32 PlayerId, int32 FunctionalIndex) override;
};
