// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "LerpData.generated.h"

UENUM(BlueprintType)
enum class ELerpSpace : uint8
{
	WorldSpace UMETA(DisplayName = "WorldSpace"),
    LocalSpace UMETA(DisplayName = "LocalSpace"),
};

USTRUCT()
struct FVectorLerpData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	USceneComponent* TargetComponent;

	ELerpSpace LerpSpace;
	bool bIsCompleted;
	float InterpSpeed;

	FVector Start;
	FVector Target;

	FVectorLerpData(): TargetComponent(nullptr), LerpSpace(), bIsCompleted(false), InterpSpeed(0) {}

	FVectorLerpData(USceneComponent* InTargetComponent, const ELerpSpace InLerpSpace,
	                const float InInterpSpeed, const FVector InTarget)
	{

		if (!InTargetComponent)
		{
			return;
		}

		this->TargetComponent = InTargetComponent;
		this->LerpSpace = InLerpSpace;
		this->Target = InTarget;
		this->InterpSpeed = InInterpSpeed;

		this->Start = GetCurrent();
		this->bIsCompleted = false;
	}

	void UpdateTarget(const FVector InTarget)
	{
		this->bIsCompleted = false;
		this->Target = InTarget;
	}

	FVector GetCurrent() const
	{
		return LerpSpace == ELerpSpace::WorldSpace
            ? TargetComponent->GetComponentLocation() : TargetComponent->GetRelativeLocation();
	}

	bool PreTick() const
	{
		if (TargetComponent == nullptr)
		{
			return false;
		}

		return true;
	}
	
	void Tick(float DeltaSeconds)
	{
		if (!PreTick()) return;

		const FVector InterpolationTargetLocation =
            FMath::VInterpTo(GetCurrent(), Target, DeltaSeconds, InterpSpeed);

		switch (LerpSpace)
		{
			case ELerpSpace::WorldSpace:
			{
				TargetComponent->SetWorldLocation(InterpolationTargetLocation);
			} break;
			case ELerpSpace::LocalSpace:
			{
				TargetComponent->SetRelativeLocation(InterpolationTargetLocation);
			} break;
		}

		if (FMath::Abs(Target.Size() - GetCurrent().Size()) <=.05F)
		{
			bIsCompleted = true;
		}
	}
};

USTRUCT()
struct FRotatorLerpData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	USceneComponent* TargetComponent;

	ELerpSpace LerpSpace;
	bool bIsCompleted;
	float InterpSpeed;
	
	FRotator Start;
	FRotator Target;

	FRotatorLerpData(): TargetComponent(nullptr), LerpSpace(), bIsCompleted(false), InterpSpeed(0) {}

	FRotatorLerpData(USceneComponent* InTargetComponent, const ELerpSpace InLerpSpace,
                    const float InInterpSpeed, const FRotator InTarget)
	{

		if (!InTargetComponent)
		{
			return;
		}

		this->TargetComponent = InTargetComponent;
		this->LerpSpace = InLerpSpace;
		this->Target = InTarget;
		this->InterpSpeed = InInterpSpeed;

		this->Start = GetCurrent();
		this->bIsCompleted = false;
	}

	void UpdateTarget(const FRotator InTarget)
	{
		this->bIsCompleted = false;
		this->Target = InTarget;
	}

	FRotator GetCurrent() const
	{
		return LerpSpace == ELerpSpace::WorldSpace
            ? TargetComponent->GetComponentRotation() : TargetComponent->GetRelativeRotation();
	}

	bool PreTick() const
	{
		if (TargetComponent == nullptr)
		{
			return false;
		}

		return true;
	}
	
	void Tick(float DeltaSeconds)
	{
		if (!PreTick()) return;

		const FRotator InterpolationTargetRotation =
            UKismetMathLibrary::RLerp(GetCurrent(), Target, DeltaSeconds*InterpSpeed, true);

		switch (LerpSpace)
		{
		case ELerpSpace::WorldSpace:
			{
				TargetComponent->SetWorldRotation(InterpolationTargetRotation);
			} break;
		case ELerpSpace::LocalSpace:
			{
				TargetComponent->SetRelativeRotation(InterpolationTargetRotation);
			} break;
		}

		if (GetCurrent().Equals(Target, 0.05F))
		{
			bIsCompleted = true;
		}
	}
};