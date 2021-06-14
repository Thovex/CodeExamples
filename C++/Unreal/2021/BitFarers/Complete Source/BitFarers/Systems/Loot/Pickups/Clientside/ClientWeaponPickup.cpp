// Copyright Baer and Hoggo Games


#include "ClientWeaponPickup.h"

#include "BitFarers/Macros.h"
#include "BitFarers/Characters/Player/PlayerCharacter.h"
#include "BitFarers/Characters/Player/WeaponComponent.h"
#include "BitFarers/Utilities/AccessorLibrary.h"


AClientWeaponPickup::AClientWeaponPickup()
{
	bReplicates = false;
}

void AClientWeaponPickup::SetWeaponData()
{

	USkeletalMesh* Mesh = WeaponData[0].WeaponMesh;

	RETURN_IF_NULLPTR(Mesh);
	
	this->PickupMesh->SetSkeletalMesh(Mesh);

	/*
	if (Meshes.Num() > 1)
	{
		for (int i = 1; i < Meshes.Num(); i++)
		{
			UStaticMeshComponent* StaticMeshComp = NewObject<UStaticMeshComponent>(this);
			StaticMeshComp->RegisterComponent();

			StaticMeshComp->SetStaticMesh(Meshes[i]);
			StaticMeshComp->AttachToComponent(PickupMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
		}
	}
	*/
}

void AClientWeaponPickup::InitializeWeapon_Implementation(const TArray<FWeaponStats>& InWeaponData)
{
	WeaponData = InWeaponData;
	SetWeaponData();
}

void AClientWeaponPickup::BeginPlay()
{
	Super::BeginPlay();

	PickupRadius->SetSphereRadius(50, true);
}

bool AClientWeaponPickup::Prerequisites()
{
	return true;
}

void AClientWeaponPickup::ExecutePickup()
{
	Super::ExecutePickup();

	if (InterpolationTarget)
	{
		InterpolationTarget->WeaponComponent->EquipWeapon_Request(WeaponData);
	}
}


void AClientWeaponPickup::OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Leave empty to ignore base functionality.
}

void AClientWeaponPickup::OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Leave empty to ignore base functionality.
}

void AClientWeaponPickup::DoButtonClick_Implementation(int32 PlayerId, int32 FunctionalIndex)
{
	bOnPickup = true;
	bPickupAble = true;
	InterpolationTarget = UAccessorLibrary::GetNetworkGameState(this)->GetPlayerFromId(PlayerId);
	SetActorTickEnabled(true);
}



