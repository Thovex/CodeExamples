// Copyright 2018 StrateGis Groep B.V. All Rights Reserved.
// Struct FOnlineSubSystemKey created by Jesse van Vliet

#pragma once

#include "CoreMinimal.h"
#include "UnrealNetwork.h"
#include "Online.h"
#include <FindSessionsCallbackProxy.h>
#include "FOnlineSubSystemKey.generated.h"

USTRUCT()
struct STRATEGISGAMEWORK_API FOnlineSubSystemKey
{

	GENERATED_USTRUCT_BODY()

public:
	bool ValidKey;
	IOnlineSessionPtr Sessions;
	TSharedPtr<const FUniqueNetId> UniqueNetId;

	FOnlineSubSystemKey();
	FOnlineSubSystemKey( bool ValidKey, IOnlineSessionPtr Sessions, TSharedPtr<const FUniqueNetId> UniqueNetId );

	/**
	* Validates first OnlineSubSystem and returns designated variables in a custom struct. Gets called on every session function
	* @return FOnlineSubSystemKey*
	*/
	FOnlineSubSystemKey* SubSystemValidator();
};