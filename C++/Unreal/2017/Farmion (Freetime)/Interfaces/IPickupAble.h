// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#pragma once

#include "IPickupAble.generated.h"

UINTERFACE(Blueprintable)
class UIPickupAble : public UInterface {
	GENERATED_BODY()
};

class IIPickupAble {
	GENERATED_BODY()

public:
	void PickUp(USkeletalMeshComponent* HoldingCharacterMesh);
	void Drop();
};