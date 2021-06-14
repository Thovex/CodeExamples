// Copyright Baer and Hoggo Games


#include "LerpComponent.h"


ULerpComponent::ULerpComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Internal_VectorLerp.Empty();
}

bool ULerpComponent::HasVectorInstance(const USceneComponent* TargetComponent, int32& Index)
{
	Index = -1;
	
	for (int32 i = 0; i < Internal_VectorLerp.Num(); i++)
	{
		if (Internal_VectorLerp[i].TargetComponent && Internal_VectorLerp[i].TargetComponent == TargetComponent)
		{
			Index = i;
			return true;
		}
	}
	
	return false;
}

bool ULerpComponent::HasRotatorInstance(const USceneComponent* TargetComponent, int32& Index)
{
	Index = -1;
	
	for (int32 i = 0; i < Internal_RotatorLerp.Num(); i++)
	{
		if (Internal_RotatorLerp[i].TargetComponent && Internal_RotatorLerp[i].TargetComponent == TargetComponent)
		{
			Index = i;
			return true;
		}
	}
	
	return false;
}


void ULerpComponent::AddVectorLerpInstance(USceneComponent* TargetComponent,
	const float InterpSpeed, const FVector LerpEndTarget, const ELerpSpace LerpSpace)
{
	int32 Index;
	if (HasVectorInstance(TargetComponent, Index))
	{
		Internal_VectorLerp[Index].UpdateTarget(LerpEndTarget);
		return;
	}

	Internal_VectorLerp.Add(FVectorLerpData(TargetComponent, LerpSpace, InterpSpeed, LerpEndTarget));
}

void ULerpComponent::AddRotatorLerpInstance(USceneComponent* TargetComponent, const float InterpSpeed,
	const FRotator LerpEndTarget, const ELerpSpace LerpSpace)
{
	int32 Index;
	if (HasRotatorInstance(TargetComponent, Index))
	{
		Internal_RotatorLerp[Index].UpdateTarget(LerpEndTarget);
		return;
	}

	Internal_RotatorLerp.Add(FRotatorLerpData(TargetComponent, LerpSpace, InterpSpeed, LerpEndTarget));
}


void ULerpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int32 i = 0; i < Internal_VectorLerp.Num(); i++)
	{
		Internal_VectorLerp[i].Tick(DeltaTime);
		if (Internal_VectorLerp[i].bIsCompleted)
		{
			Internal_VectorLerp.RemoveAt(i);
		}
	}

	for (int32 i = 0; i < Internal_RotatorLerp.Num(); i++)
	{
		Internal_RotatorLerp[i].Tick(DeltaTime);
		if (Internal_RotatorLerp[i].bIsCompleted)
		{
			Internal_RotatorLerp.RemoveAt(i);
		}
	}
}
