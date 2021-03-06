// Copyright Baer and Hoggo Games

#include "SlurpPickup.h"


#include "BitFarers/Macros.h"
#include "BitFarers/Characters/Player/PlayerCharacter.h"
#include "BitFarers/Utilities/MathsLibrary.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

ASlurpPickup::ASlurpPickup()
{
	PrimaryActorTick.bCanEverTick = true;

	PickupMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("PickupMesh"));
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = PickupMesh;

	PickupRadius = CreateDefaultSubobject<USphereComponent>(FName("PickupRadius"));
	PickupRadius->SetupAttachment(PickupMesh);
	PickupRadius->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupRadius->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Overlap);
	PickupRadius->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	PickupRadius->SetSphereRadius(100, false);
}

void ASlurpPickup::StartInterpolation()
{
	InterpolationTarget->TransferOwnership(this);

	GetWorldTimerManager().
		SetTimer(
			InterpolationTimer,
			this,
			&ASlurpPickup::Interpolator,
			GetWorld()->GetDeltaSeconds(),
			true,
			-1
		);

	PickupRadius->DestroyComponent();
}

void ASlurpPickup::SetRotationToMaterials(const FVector Rotation, const float Speed)
{
	for (auto& Material : PickupMeshMaterials)
	{
		Material->SetVectorParameterValue(FName("Rotation"), FLinearColor(Rotation.X, Rotation.Y, Rotation.Z, 0));
		Material->SetScalarParameterValue(FName("Speed"), Speed);
	}
}


EAmmoType ASlurpPickup::ClassifyAmmoType(const FString ItemName)
{
	if (ItemName == "Light_Ammo")
	{
		return EAmmoType::Light;
	}

	if (ItemName == "Medium_Ammo")
	{
		return EAmmoType::Medium;
	}

	if (ItemName == "Energy_Ammo")
	{
		return EAmmoType::Energy;
	}

	if (ItemName == "Heavy_Ammo")
	{
		return EAmmoType::Heavy;
	}

	if (ItemName.ToLower().Contains("ammo"))
	{
		PRINT("You're likely missing an ammo type or misspelling ammo types, please double-check!");
	}

	return static_cast<EAmmoType>(-1);
}

void ASlurpPickup::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	PickupRadius->OnComponentBeginOverlap.AddDynamic(this, &ASlurpPickup::OnPickupOverlapBegin);
	PickupRadius->OnComponentEndOverlap.AddDynamic(this, &ASlurpPickup::OnPickupOverlapEnd);
}

void ASlurpPickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsPickingUp && bOnPickup && bPickupAble && InterpolationTarget)
	{
		if (Prerequisites())
		{
			bIsPickingUp = true;
			StartInterpolation();
			SetActorTickEnabled(false);
		}
	}
}

bool ASlurpPickup::Prerequisites()
{
	return true;
}

void ASlurpPickup::ExecutePickup()
{
}

void ASlurpPickup::ClearIdle()
{
	bPickupAble = true;
}

void ASlurpPickup::InitializeMaterials()
{
	PickupMeshMaterials.Empty();

	int32 i = 0;
	for (auto& Material : PickupMesh->GetMaterials())
	{
		auto* DynamicMaterialInst = UMaterialInstanceDynamic::Create(Material, PickupMesh);
		PickupMesh->SetMaterial(i, DynamicMaterialInst);
		PickupMeshMaterials.Add(DynamicMaterialInst);
		i++;
	}
}

void ASlurpPickup::HandleIdleTime()
{
	if (IdleTime > 0)
	{
		FTimerHandle IdleHandler;
		GetWorldTimerManager().SetTimer(IdleHandler, this, &ASlurpPickup::ClearIdle, IdleTime, false);
	}
	else
	{
		bPickupAble = true;
	}
}

void ASlurpPickup::Interpolator()
{
	const FVector CurrentLocation = GetActorLocation();
	const FVector EndLocation = InterpolationTarget->PlayerBodySM->GetComponentLocation();

	SetActorLocation(
		FMath::Lerp(CurrentLocation, EndLocation, GetWorld()->GetDeltaSeconds() * 5),
		false, nullptr, ETeleportType::TeleportPhysics);

	const float Distance = (CurrentLocation - EndLocation).Size();

	if (Distance < DistanceToScale)
	{
		const float Scale = UMathsLibrary::SetScale(Distance, 0, 500, 0, 1);
		SetActorScale3D(FVector::OneVector * Scale);
	}

	if (Distance < DistanceToPickup)
	{
		if (!bIsPickedUp)
		{
			ExecutePickup();
			bIsPickedUp = true;
		}

		if (GetIsReplicated())
		{
			DestroyAuthority();
		}
		else
		{
			Destroy();
		}
	}
}


void ASlurpPickup::DestroyAuthority_Implementation()
{
	Destroy();
}

void ASlurpPickup::OnPickupOverlapBegin(
	class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (OtherActor->IsA<APlayerCharacter>())
	{
		const auto LocalPlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (LocalPlayerController->GetPawn() == OtherActor)
		{
			bOnPickup = true;
			InterpolationTarget = Cast<APlayerCharacter>(OtherActor);
			SetActorTickEnabled(true);
		}
	}
}

void ASlurpPickup::OnPickupOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<APlayerCharacter>())
	{
		const auto LocalPlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (LocalPlayerController->GetPawn() == OtherActor)
		{
			bOnPickup = false;
			SetActorTickEnabled(false);
		}
	}
}
