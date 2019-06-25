// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Items/ItemBase.h"
#include "Objects/ResourceBase.h"
#include "FarmionCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(FarmionCharacterLog, Log, All);

class UFarmionCharacterBuilding;

UCLASS(config = Game)
class AFarmionCharacter : public ACharacter {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ))
		class UCameraComponent* FollowCamera;
public:
	AFarmionCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(EditAnywhere, Category = Interact)
		int32 InteractRange;

	UPROPERTY(EditAnywhere, Category = Throw)
		int32 ThrowForce;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Interact)
		bool bIsHoldingItem;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Interact)
		AItemBase* CurrentHeldItem;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Target)
		AResourceBase* ResourceTarget;

	UStaticMeshComponent* PreviewBuildingMeshComp;
	UStaticMeshComponent* CurrentHeldItemMeshComp;
	TArray<USceneComponent*> IKComponents;

	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> LastRemovedCollisionActors;
	TArray<AActor*> LastAddedCollisionActors;
	TArray<FHitResult> HitActors;

	AActor* LastIgnored;

	bool bBuildingPressedState;

	UFarmionCharacterBuilding* FarmionBuilding;

	FVector CameraToCursorVector(int32 VectorLength, bool StartVector) const;
	FHitResult CameraToCursorLT(int32 LT_Distance, bool Debug) const;

	UFUNCTION(BlueprintNativeEvent, Category = Animation)
		void PushToAnimator(FName BoolName, bool BoolValue);

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	
private:
	void Interact();

	void InteractPress();
	void InteractPickup();

	UFUNCTION(reliable, server, WithValidation)
		void ServerInteractPress();

	UFUNCTION(reliable, server, WithValidation)
		void ServerInteractPickup();

	void InteractDrop(bool bIsTossed);

	UFUNCTION(reliable, server, WithValidation)
		void ServerInteractDropData(bool bIsTossed);

	UFUNCTION(reliable, server, WithValidation)
		void ServerStartWoodcutting();

	UFUNCTION(reliable, server, WithValidation)
		void ServerStopWoodcutting();

	UFUNCTION(reliable, NetMulticast, WithValidation)
		void MulticastPushToAnimator(FName BoolName, bool BoolValue);

	void Throw();
	void Build();
	void SetIKData();
	void PlayerBelowObjectEnableCollision();

	void MWheelDown();
	void MWheelUp();

	void LeftMouseButton();
	void LeftMouseButtonRelease();

	bool bIsWoodcutting;
	bool bIsMining;


public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(reliable, server, WithValidation)
		void ServerLeftMouseEvent();

	bool LeftMouseButtonDown;

};
