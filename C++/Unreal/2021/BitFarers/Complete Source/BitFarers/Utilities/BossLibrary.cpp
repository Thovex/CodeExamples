// Copyright Baer and Hoggo Games


#include "BossLibrary.h"


#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "BitFarers/Macros.h"

TMap<int32, FVectorList> UBossLibrary::CircleCast(const UObject* WorldContextObject, USceneComponent* BaseComponent,
                                                  int32 Divider, int32 ForRadial, int32 PosIncr, int32 RadIncr)
{
	TMap<int32, FVectorList> CircleTraces;

	int32 Pos = 0, Rad = RadIncr;

	for (int32 i = 1; i <= ForRadial; i++)
	{
		if (Pos >= 360)
		{
			Pos = 0;
			Rad += RadIncr;
		} else
		{
			Pos += PosIncr;
		}

		const UWorld* World = WorldContextObject->GetWorld();

		FVector StartVector = BaseComponent->GetComponentLocation() + (BaseComponent->GetUpVector() * 5000);
		StartVector.X += UKismetMathLibrary::DegCos(float(Pos)) * float(Rad);
		StartVector.Y += UKismetMathLibrary::DegSin(float(Pos)) * float(Rad);
		
		FVector EndVector =
			StartVector + (FVector::UpVector*-1 * 50000);

		FHitResult HitResult;
		World->LineTraceSingleByChannel(HitResult, StartVector, EndVector, ECC_Visibility);

		if (HitResult.bBlockingHit)
		{
			//DrawDebugLine(World, HitResult.TraceStart, HitResult.ImpactPoint, FColor::Red, false, 5,0, 5);

			if (CircleTraces.Contains(i/Divider))
			{
				auto& Array = CircleTraces[i/Divider];
				Array.Vectors.Add(HitResult.ImpactPoint);
			} else
			{
				auto Array = FVectorList();
				Array.Vectors.Add(HitResult.ImpactPoint);
				CircleTraces.Add(i/Divider, Array);
			}

		} else
		{
			//DrawDebugLine(World, HitResult.TraceStart, HitResult.TraceEnd, FColor::Cyan, false, 5,0, 5);

		}

	}

	return CircleTraces;

}

FString UBossLibrary::VectorArrayToFString(const UObject* WorldContextObject, const TArray<FVector> InVectors)
{
	FString VectorString = FString();
	
	for (FVector V : InVectors)
	{
		VectorString.Append(V.ToCompactString());
	}

	return VectorString;
}

FString UBossLibrary::VectorToFString(const UObject* WorldContextObject, const FVector InVector)
{

	return InVector.ToCompactString();
}

TArray<FVector> UBossLibrary::FStringToVectorArray(const UObject* WorldContextObject, const FString InString)
{
	TArray<FVector> Results;
	
	const FRegexPattern VectorPattern(TEXT("\\V\\(X=(.+?),\\s+Y=(.+?),\\s+Z=(.+?)\\)"));
	FRegexMatcher VectorMatcher(VectorPattern, InString);

	while (VectorMatcher.FindNext())
	{
		FString String = VectorMatcher.GetCaptureGroup(0);
		FVector Vector;
		bool Valid;
		
		UKismetStringLibrary::Conv_StringToVector(String, Vector, Valid);

		if (Valid)
		{
			Results.Add(Vector);
		}
	}

	return Results;
}
