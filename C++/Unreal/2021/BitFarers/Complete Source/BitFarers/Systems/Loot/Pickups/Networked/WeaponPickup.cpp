// Copyright Baer and Hoggo Games


#include "WeaponPickup.h"

#include "BitFarers/Macros.h"
#include "BitFarers/Characters/Player/WeaponComponent.h"
#include "BitFarers/Utilities/AccessorLibrary.h"
#include "Net/UnrealNetwork.h"


AWeaponPickup::AWeaponPickup()
{
	bReplicates = true;
}

void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();

	PickupRadius->SetSphereRadius(50, true);
}

bool AWeaponPickup::Prerequisites()
{
	return true;
}

void AWeaponPickup::InitializeWeapon_Implementation(const TArray<FWeaponStats>& InWeaponData)
{
	InitializeWeapon_Response(InWeaponData);
}

void AWeaponPickup::InitializeWeapon_Response_Implementation(const TArray<FWeaponStats>& InWeaponData)
{
	this->WeaponData = InWeaponData;
	OnRep_SetWeaponData();
}

void AWeaponPickup::OnRep_SetWeaponData()
{
	// note. Preferably only set this once, otherwise - add removal code for previous meshes.

	// We probably only need the 0th index WeaponMesh..
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

void AWeaponPickup::ExecutePickup()
{
	Super::ExecutePickup();

	if (InterpolationTarget)
	{
		InterpolationTarget->WeaponComponent->EquipWeapon_Request(WeaponData);
		Destroy();
	}
}

void AWeaponPickup::OnPickupOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Leave empty to ignore base functionality.
}

void AWeaponPickup::OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Leave empty to ignore base functionality.
}


void AWeaponPickup::DoButtonClick_Implementation(int32 PlayerId, int32 FunctionalIndex)
{	
	bOnPickup = true;
	bPickupAble = true;
	InterpolationTarget = UAccessorLibrary::GetNetworkGameState(this)->GetPlayerFromId(PlayerId);
	
	SetActorTickEnabled(true);
}


void AWeaponPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponPickup, WeaponData);
}