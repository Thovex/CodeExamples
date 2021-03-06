// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#pragma once

#include "CoreMinimal.h"
#include "Common/AuthorityActor.h"
#include "Items/BuildingBookBase.h"
#include "Buildings/BuildingBaseSphereComponent.h"
#include "BuildingBase.generated.h"

UCLASS()
class FARMION_API ABuildingBase : public AAuthorityActor {
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	ABuildingBase(const FObjectInitializer & ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* BuildingMeshComponent;

	UPROPERTY(ReplicatedUsing = OnRep_SetCollisionArray)
		TArray<UBuildingBaseSphereComponent*> SphereCollisions;

	UPROPERTY(ReplicatedUsing = OnRep_SetBuildingMesh)
		UStaticMesh* BuildingMesh;

	UPROPERTY(Replicated)
		EBuildingBookType BuildingType;

	UPROPERTY(Replicated)
		TArray<EBuildingBookType> TypesLookingAt;

	bool bMadeSpheres;

	UPROPERTY(Replicated)
		bool bIsSelected;

	void AddType(EBuildingBookType TypeToAdd);
	void RemoveSphere(UBuildingBaseSphereComponent* SphereToRemove);

	UFUNCTION(reliable, server, WithValidation)
		void ServerRemoveSphere(UBuildingBaseSphereComponent* SphereToRemove);
	 
	UFUNCTION()
		void OnRep_SetBuildingMesh();

	UFUNCTION()
		void OnRep_SetCollisionArray();

protected:
	UFUNCTION(reliable, server, WithValidation)
		void ServerCreateSpheres();

	UFUNCTION(reliable, server, WithValidation)
		void ServerAddSphere(FName SocketName, FTransform SocketTransform);

	void CheckForSelection(float DeltaTime);
	void ActivateSelection();


	void ServerTick(float DeltaTime);
	void ClientTick(float DeltaTime);

private:
	TArray<EBuildingBookType> AllowedTypes = {
		EBuildingBookType::EBBT_Bases,
		EBuildingBookType::EBBT_Foundations,
		EBuildingBookType::EBBT_Walls,
		EBuildingBookType::EBBT_Doors
	};

	UPROPERTY(Replicated)
		float SelectedTimer;

	UPROPERTY()
		TArray<UMaterialInterface*> OriginalMaterials;

	UMaterial* BuildingMaterial;
	UClass* BuildingClass;

	void ReturnMaterials();

};
