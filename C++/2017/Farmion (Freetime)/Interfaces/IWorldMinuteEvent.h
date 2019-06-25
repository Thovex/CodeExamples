// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#pragma once

#include "CoreMinimal.h"
#include "IWorldMinuteEvent.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIWorldMinuteEvent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FARMION_API IIWorldMinuteEvent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	void MinuteEvent();
	
};
