// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KioTriggerIdentity.h"
#include "KioTriggerBase.generated.h"

class AKioTriggerEntityBase;
class AKioCharacter;

UENUM( BlueprintType )
enum class ETriggerType : uint8 {
	ETT_SingleTrigger                         UMETA( DisplayName = "Single Trigger" ),
	ETT_MultipleTrigger                       UMETA( DisplayName = "Multiple Trigger" ),
	ETT_DelayedSingleTrigger	              UMETA( DisplayName = "Delayed Single Trigger" ),
	ETT_DelayedMultipleTriggerSameTime	      UMETA( DisplayName = "Delayed Multiple Trigger Same Time" ),
	ETT_DelayedMultipleTriggerVaryingTimes    UMETA( DisplayName = "Delayed Multiple Trigger Varying Times" ),
	ETT_RequireItemTrigger                    UMETA( DisplayName = "Require Item Trigger" ),
	ETT_FakeTrigger                           UMETA( DisplayName = "Fake Trigger" ),
	ETT_TriggerButton                         UMETA( DisplayName = "Trigger Button" )
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FTriggerHitDelegate, AActor*, Player );

UCLASS()
class KIO_API AKioTriggerBase : public AActor {
	GENERATED_BODY()

public:
	AKioTriggerBase( const FObjectInitializer & ObjectInitializer );

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Identity" )
		class UKioTriggerIdentity* TriggerIdentityData;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Trigger Settings" )
		ETriggerType TriggerType;

	UPROPERTY( BlueprintReadWrite, VisibleAnywhere, Category = "Trigger Settings" )
		class UBoxComponent* TriggerBoxCollision;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Trigger Settings" )
		TArray<TSubclassOf<AActor>> ActorClassesToCallTrigger;

	UPROPERTY( BlueprintAssignable, Category = "Trigger" )
		FTriggerHitDelegate OnTriggerHit;

public:

	UBoxComponent* GetTriggerCollision() const;
	UFUNCTION( BlueprintCallable ) FTriggerIdentityData GetTriggerIdentity() const;


protected:
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;

	virtual void Trigger( AActor* HitActor );

	void BindDelegates();
	void TriggerBeginCheck();

	UFUNCTION()
		virtual void BeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult );

	UFUNCTION()
		virtual void EndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex );

	bool ClassCheck( AActor* OtherActor );

};
