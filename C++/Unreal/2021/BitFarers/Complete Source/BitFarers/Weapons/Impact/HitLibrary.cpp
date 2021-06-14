// Copyright Baer and Hoggo Games


#include "HitLibrary.h"

#include "ImpactType.h"
#include "Components/DecalComponent.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BitFarers/Macros.h"


void UHitLibrary::CreateParticleAndDecal(const UObject* WorldContextObject, const FVector Location, const FVector Normal, USceneComponent* HitComponent, FImpactRow* ImpactRow)
{
	RETURN_IF_NULLPTR(ImpactRow);


	if (ImpactRow->ImpactSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
         WorldContextObject, ImpactRow->ImpactSystem, Location, UKismetMathLibrary::MakeRotFromZ(Normal),
         ImpactRow->ImpactSystemScale);
	}

	if (ImpactRow->NImpactSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(WorldContextObject, ImpactRow->NImpactSystem, Location,
			UKismetMathLibrary::MakeRotFromZ(Normal), ImpactRow->NImpactSystemScale);
	}

	UDecalComponent* Decal;
	
	FRotator Rot = UKismetMathLibrary::MakeRotFromX(Normal) * -1;
	Rot.Roll = FMath::RandRange(0, 360);
	
	if (HitComponent)
	{
		Decal = UGameplayStatics::SpawnDecalAttached(
            ImpactRow->ImpactDecal, FVector::OneVector * ImpactRow->ImpactDecalScale, HitComponent, NAME_None, Location, Rot,
            EAttachLocation::KeepWorldPosition, 3.0F);

	} else
	{
		Decal = UGameplayStatics::SpawnDecalAtLocation(WorldContextObject, ImpactRow->ImpactDecal,
            FVector::OneVector * ImpactRow->ImpactDecalScale, Location, Rot, 3.0F);
	}

	if (Decal) {
		Decal->SetFadeScreenSize(0.002f);
		Decal->SetFadeOut(3, 1, false);
		Decal->SetRelativeRotation(Rot);
	}
}

void UHitLibrary::SpawnHitEmitter(const UObject* WorldContextObject, const UDataTable* DT,
                                  const FVector Location, const FVector Normal, USceneComponent* HitComponent)
{
	bool bHasPlayed = false;
	const FString RequesterWarning;

	if (HitComponent)
	{
		if (HitComponent->ComponentTags.Num() > 0)
		{
			for (auto& Tag : HitComponent->ComponentTags)
			{
				FImpactRow* ImpactRow = DT->FindRow<FImpactRow>(Tag, RequesterWarning, false);

				if (ImpactRow)
				{
					CreateParticleAndDecal(WorldContextObject, Location, Normal, HitComponent, ImpactRow);
					bHasPlayed = true;
				}
			}
		}
	}

	
	if (!bHasPlayed)
	{
		FImpactRow* ImpactRow = DT->FindRow<FImpactRow>("Default", RequesterWarning, false);

		if (ImpactRow)
		{
			CreateParticleAndDecal(WorldContextObject, Location, Normal, HitComponent, ImpactRow);
			bHasPlayed = true;
		}
	}
}

FVector UHitLibrary::CalculateNormalFromDirections(const FVector V1,
	const FVector V2)
{
	return (V2 - V1).GetSafeNormal();
}
