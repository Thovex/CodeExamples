// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"


#include "BitFarers/Utilities/Interfaces/ButtonClickable.h"
#include "BitFarers/Systems/Loot/Pickups/SlurpPickup.h"
#include "BitFarers/Weapons/WeaponStats.h"


#include "ClientWeaponPickup.generated.h"

UCLASS()
class BITFARERS_API AClientWeaponPickup final : public ASlurpPickup, public IButtonClickable
{
	GENERATED_BODY()

public:
	AClientWeaponPickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	TArray<FWeaponStats> WeaponData;

    void SetWeaponData();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Pickup")
    void InitializeWeapon(const TArray<FWeaponStats>& InWeaponData);


protected:
	virtual void BeginPlay() override;
	virtual bool Prerequisites() override;
	virtual void ExecutePickup() override;

	virtual void OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	
public:
	virtual void DoButtonClick_Implementation(int32 PlayerId, int32 FunctionalIndex) override;

};
