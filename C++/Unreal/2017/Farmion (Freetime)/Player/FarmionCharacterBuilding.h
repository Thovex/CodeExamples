// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/BuildingBookBase.h"
#include "FarmionCharacter.h"
#include "FarmionMacros.h"
#include "Engine.h"
#include "UnrealNetwork.h"
#include "Buildings/BuildingBaseSphereComponent.h"
#include "FarmionCharacterBuilding.generated.h"

/* CUSTOM BUILD STATUS ENUM */
UENUM(BlueprintType)
enum class EBuildingState : uint8 {
	EBS_None	 			UMETA(DisplayName = "Is None State"),
	EBS_Selecting 			UMETA(DisplayName = "Is Selecting"),
	EBS_Building			UMETA(DisplayName = "Is Building")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FARMION_API UFarmionCharacterBuilding : public UActorComponent {
	GENERATED_BODY()

private:
/* DEFAULT PRIVATE VARIABLES - internal use only */
	
	float SlowTickrate;

public:
/* DEFAULT PUBLIC VARIABLES - external and internal use */
	
	FVector CurrentLocation;

private:
/* PRIVATE CONSTRUCTOR NON-DESTROY VARIABLES - internal use only */

	UPROPERTY()
		UMaterial* BuildingMaterialGood;

	UPROPERTY()
		UMaterialInstanceDynamic* BuildingMaterialGoodDyn;

	UPROPERTY()
		UMaterial* BuildingMaterialBad;

	UPROPERTY()
		UMaterialInstanceDynamic* BuildingMaterialBadDyn;

private:
	/* PRIVATE REPLICATED VARIABLES - internal use only */

	/* Cooldown Timer, only holds variable. Time is hardcoded. */
	UPROPERTY(Replicated)
		float BuildCooldownTimer;

	/* Bool if "Is Building". */
	UPROPERTY(Replicated)
		bool bBuildActive;

	/* Bool if camera has to start lerping. */
	UPROPERTY(Replicated)
		bool bCameraLerp;

	/* Location to lerp to. */
	UPROPERTY(Replicated)
		FVector CameraBoomLerpLocation;

	/* Current state of building. See enum for details. */
	UPROPERTY(Replicated)
		EBuildingState ECurrentBuildingState;

	/* RepNotify, changes happen on server only. */
	UPROPERTY(ReplicatedUsing = OnRep_SetMesh)
		UStaticMesh* SelectedMesh;

	/* RepNotify, changes happen on server only. - Clientside is fake. */
	UPROPERTY(ReplicatedUsing = OnRep_SetLocation)
		FVector SelectedLocation;

	/* RepNotify, changes happen on server only. */
	UPROPERTY(ReplicatedUsing = OnRep_SetRotation)
		FRotator SelectedRotation;

	/* RepNotify, changes happen on server only. */
	UPROPERTY(ReplicatedUsing = OnRep_SetVisibility)
		bool SelectedVisibility;

	/* RepNotify, changes happen on server only. */
	UPROPERTY(ReplicatedUsing = OnRep_SetCollision)
		bool SelectedCollision;

	/* Snapping building sphere that is being selected. */
	UPROPERTY(Replicated)
		UBuildingBaseSphereComponent* SelectedSphere;

	/* Players current held item casted to ABuildingBookBase */
	UPROPERTY(Replicated)
		ABuildingBookBase* CurrentBook;

	/* Current Owner casted to AFarmionCharacter */
	UPROPERTY(Replicated)
		AFarmionCharacter* CurrentOwner;

	/* Players building preview mesh */
	UPROPERTY(Replicated)
		UStaticMeshComponent* CurrentOwnerPreviewMesh;

protected:
/* PROTECTED INHERITED FUNCTIONS */
	UFarmionCharacterBuilding();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
/* PRIVATE BLUEPRINT FUNCTIONS - internal use only */
	
	UFUNCTION(BlueprintCallable, Category = "Farmion Building")
		void SelectMesh(UStaticMesh* TheMesh);

public:
/* PUBLIC FUNCTIONS - external use only */

	void PressB(AFarmionCharacter* TheOwner);
	void BuildObject(AFarmionCharacter* TheOwner);


private:
/* PRIVATE FUNCTIONS - internal use only */
	void SetBeginValues();

	void SetInputActive();
	void SetInputInactive();

	void SlowTick();

	void DoCameraLerp();

	void ConstantCheckIfHasBook();
	void CheckIfNullMaterial();
	void CheckPreviewCollisions();

	void ClientsideLocationCheck(float DeltaTime);

private:
/* PRIVATE REPNOTIFY FUNCTIONS - internal use only */
	
	UFUNCTION()
		void OnRep_SetMesh();

	UFUNCTION()
		void OnRep_SetCollision();

	UFUNCTION()
		void OnRep_SetRotation();

	UFUNCTION()
		void OnRep_SetVisibility();

	UFUNCTION()
		void OnRep_SetLocation();

public:
/* PUBLIC SERVER FUNCTIONS - external use only */

	UFUNCTION(reliable, server, WithValidation)
		void ServerRotateObject(FRotator PlusRotation);


private:
/* PRIVATE SERVER FUNCTIONS - internal use only */

	UFUNCTION(reliable, server, WithValidation)
		void ServerCheckPreviewCollisions(bool Active);
	
	UFUNCTION(reliable, server, WithValidation)
		void ServerSetCameraBoomHigh(AFarmionCharacter* TheOwner);
	
	UFUNCTION(reliable, server, WithValidation)
		void ServerSetCameraBoomLow(AFarmionCharacter* TheOwner);

	UFUNCTION(reliable, server, WithValidation)
		void ServerSetMesh(AFarmionCharacter* TheOwner, UStaticMesh* TheMesh);

	UFUNCTION(reliable, server, WithValidation)
		void ServerSetLocation(AFarmionCharacter* TheOwner);

	UFUNCTION(reliable, server, WithValidation)
		void ServerBuildObject(AFarmionCharacter* TheOwner, FVector TheLocation, FRotator TheRotation, FVector TheScale, UStaticMesh* TheMesh, EBuildingBookType TheBuildingBookType, UBuildingBaseSphereComponent* TheSphere);

	UFUNCTION(reliable, server, WithValidation)
		void ServerPressB(AFarmionCharacter* TheOwner);

private:
/* PRIVATE MULTICAST FUNCTIONS - internal use only */

	UFUNCTION(reliable, NetMulticast, WithValidation)
		void MulticastGetBook(AFarmionCharacter* TheOwner);

	UFUNCTION(reliable, NetMulticast, WithValidation)
		void MulticastClearBook(AFarmionCharacter* TheOwner);

public:
/* PUBLIC CLIENT FUNCTIONS - internal use only */

	UFUNCTION(reliable, client, WithValidation)
		void ClientOpenBook(AFarmionCharacter* TheOwner);

	UFUNCTION(reliable, client, WithValidation)
		void ClientCloseBook(AFarmionCharacter* TheOwner);

};
