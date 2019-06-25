// Copyright 2018 StrateGis Groep B.V. All Rights Reserved.
// Struct FOnlineSubSystemKey created by Jesse van Vliet

#include "FOnlineSubSystemKey.h"

FOnlineSubSystemKey::FOnlineSubSystemKey()
{
	this->ValidKey = false;
	this->Sessions = nullptr;
	this->UniqueNetId = nullptr;
}

FOnlineSubSystemKey::FOnlineSubSystemKey( bool ValidKey, IOnlineSessionPtr Sessions, TSharedPtr<const FUniqueNetId> UniqueNetId )
{
	this->ValidKey = ValidKey;
	this->Sessions = Sessions;
	this->UniqueNetId = UniqueNetId;
}

FOnlineSubSystemKey* FOnlineSubSystemKey::SubSystemValidator()
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	TSharedPtr<const FUniqueNetId> UniqueNetId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId( 0 );

	FOnlineSubSystemKey* OnlineKey = new FOnlineSubSystemKey;

	if ( OnlineSub )
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if ( Sessions.IsValid() && UniqueNetId.IsValid() )
		{
			OnlineKey->ValidKey = true;
			OnlineKey->Sessions = Sessions;
			OnlineKey->UniqueNetId = UniqueNetId;

			return OnlineKey;
		}
	}

	return OnlineKey;
}
