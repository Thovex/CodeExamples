// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#include "FarmionMainGameInstance.h"

UFarmionMainGameInstance::UFarmionMainGameInstance(const FObjectInitializer & ObjectInitializer) {
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UFarmionMainGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UFarmionMainGameInstance::OnStartOnlineGameComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UFarmionMainGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UFarmionMainGameInstance::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UFarmionMainGameInstance::OnDestroySessionComplete);
}

void UFarmionMainGameInstance::StartOnlineGame(bool bIsLan, bool bIsPresence, int32 NumPlayers, FName ServerName, FName MapName, FName GamemodeName) {
	ULocalPlayer* const Player = GetFirstGamePlayer();
	HostSessionBool(Player->GetPreferredUniqueNetId(), GameSessionName, ServerName, MapName, GamemodeName, bIsLan, bIsPresence, NumPlayers);
	CreatedGame = true;
}

void UFarmionMainGameInstance::FindOnlineGames(bool bIsLan, bool bIsPresence, UClass* UserWidgetClass, UScrollBox* ScrollBox) {
	ULocalPlayer* const Player = GetFirstGamePlayer();

	ServerList = ScrollBox;
	WidgetInstanceClass = UserWidgetClass;

	FindSessions(Player->GetPreferredUniqueNetId(), bIsLan, bIsPresence);
}

void UFarmionMainGameInstance::JoinOnlineGame(FBlueprintSessionResult BlueprintSearchResult) {
	ULocalPlayer* const Player = GetFirstGamePlayer();

	FOnlineSessionSearchResult SessionSearch = BlueprintSearchResult.OnlineResult;

	if(SessionSearch.Session.OwningUserId != Player->GetPreferredUniqueNetId()) {
		JoinSessionBool(Player->GetPreferredUniqueNetId(), GameSessionName, SessionSearch);
	}
	CreatedGame = false;
}

void UFarmionMainGameInstance::DestroySessionAndLeaveGame() {
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if(OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if(Sessions.IsValid()) {
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			Sessions->DestroySession(GameSessionName);
		}
	}
	CreatedGame = false;
}

bool UFarmionMainGameInstance::HostSessionBool(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, FName ServerName, FName MapName, FName GamemodeName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers) {
	// Get the Online Subsystem to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if(OnlineSub) {
		// Get the Session Interface, so we can call the "CreateSession" function on it
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if(Sessions.IsValid() && UserId.IsValid()) {
			/*
			Fill in all the Session Settings that we want to use.

			There are more with SessionSettings.Set(...);
			For example the Map or the GameMode/Type.
			*/
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			SessionSettings->Set(SETTING_MAPNAME, MapName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);
			SessionSettings->Set(SETTING_GAMEMODE, GamemodeName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);

			FOnlineSessionSetting CompoundSessionName;
			CompoundSessionName.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
			CompoundSessionName.Data = SessionName.ToString();
			SessionSettings->Settings.Add(FName("SESSION_NAME"), CompoundSessionName);

			// Set the delegate to the Handle of the SessionInterface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			// Our delegate should get called when this is complete (doesn't need to be successful!)

			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsytem found!"));
	}

	return false;
}

void UFarmionMainGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if(OnlineSub) {
		// Get the Session Interface to call the StartSession function
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if(Sessions.IsValid()) {
			// Clear the SessionComplete delegate handle, since we finished this call
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if(bWasSuccessful) {
				// Set the StartSession delegate handle
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				// Our StartSessionComplete delegate should get called after this
				Sessions->StartSession(SessionName);

				FNamedOnlineSession* SelectedSession = Sessions->GetNamedSession(SessionName);
				CreatedSessionName = SessionName;
				CreatedSessionPlayerName = FName(*SelectedSession->OwningUserName);


			}
		}
	}
}

void UFarmionMainGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful) {
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the Online Subsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if(OnlineSub) {
		// Get the Session Interface to clear the Delegate
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if(Sessions.IsValid()) {
			// Clear the delegate, since we are done with this call
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	// If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
	if(bWasSuccessful) {
		UGameplayStatics::OpenLevel(GetWorld(), "Developers", true, "listen");
	}
}

void UFarmionMainGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence) {
	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if(OnlineSub) {
		// Get the SessionInterface from our OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if(Sessions.IsValid() && UserId.IsValid()) {
			/*
			Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
			*/
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 50000;
			SessionSearch->PingBucketSize = 50;

			// We only want to set this Query Setting if "bIsPresence" is true
			if(bIsPresence) {
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

			// Set the Delegate to the Delegate Handle of the FindSession function
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			// Finally call the SessionInterface function. The Delegate gets called once this is finished
			Sessions->FindSessions(*UserId, SearchSettingsRef);
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Loading matches, please wait.")));


		}
	} else {
		// If something goes wrong, just call the Delegate Function directly with "false".
		OnFindSessionsComplete(false);
	}
}

void UFarmionMainGameInstance::OnFindSessionsComplete(bool bWasSuccessful) {
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), bWasSuccessful));

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if(OnlineSub) {
		// Get SessionInterface of the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if(Sessions.IsValid()) {
			// Clear the Delegate handle, since we finished this call
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			// Just debugging the Number of Search results. Can be displayed in UMG or something later on
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));

			// If we have found at least 1 session, we just going to debug them. You could add them to a list of UMG Widgets, like it is done in the BP version!
			if(SessionSearch->SearchResults.Num() > 0) {
				// "SessionSearch->SearchResults" is an Array that contains all the information. You can access the Session in this and get a lot of information.
				// This can be customized later on with your own classes to add more information that can be set and displayed
				for(int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++) {
					// OwningUserName is just the SessionName for now. I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
					// This is something you can't do in Blueprint for example!

					ServerList->ClearChildren();

					UServerBrowserItem* WidgetInstance = CreateWidget<UServerBrowserItem>(GetWorld()->GetFirstPlayerController(), WidgetInstanceClass);
					WidgetInstance->AddToViewport();

					WidgetInstance->FSessionResult.OnlineResult = SessionSearch->SearchResults[SearchIdx];

					UTextBlock* WidgetInstanceTextServerName = ( (UTextBlock*)WidgetInstance->WidgetTree->FindWidget(FName("TXT_Server")) );
					UTextBlock* WidgetInstanceTextServerPing = ( (UTextBlock*)WidgetInstance->WidgetTree->FindWidget(FName("TXT_Ping")) );
					UTextBlock* WidgetInstanceTextServerPlayers = ( (UTextBlock*)WidgetInstance->WidgetTree->FindWidget(FName("TXT_Players")) );
					UTextBlock* WidgetInstanceTextServerMapname = ( (UTextBlock*)WidgetInstance->WidgetTree->FindWidget(FName("TXT_Mapname")) );
					UTextBlock* WidgetInstanceTextServerGamemode = ( (UTextBlock*)WidgetInstance->WidgetTree->FindWidget(FName("TXT_Gamemode")) );

					FString HostName = SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.Settings.FindRef("SESSION_NAME").Data.ToString();

					WidgetInstanceTextServerName->SetText(FText::FromString(*HostName));

					int32 PingMS = SessionSearch->SearchResults[SearchIdx].PingInMs;

					WidgetInstanceTextServerPing->SetText(FText::FromString(FString::FromInt(PingMS) + " MS"));

					int32 CurrentPlayers = ( SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.NumPublicConnections - SessionSearch->SearchResults[SearchIdx].Session.NumOpenPublicConnections ) + ( SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.NumPrivateConnections - SessionSearch->SearchResults[SearchIdx].Session.NumOpenPrivateConnections );
					int32 MaxPlayers = SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.NumPublicConnections;
					int32 MaxPrivatePlayers = SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.NumPrivateConnections;

					WidgetInstanceTextServerPlayers->SetText(FText::FromString(FString::FromInt(CurrentPlayers) + " / " + FString::FromInt(MaxPlayers) + " (" + FString::FromInt(MaxPrivatePlayers) + ")"));

					FString Gamemode;
					SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.Get(SETTING_GAMEMODE, Gamemode);

					FString Mapname;
					SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.Get(SETTING_MAPNAME, Mapname);
					//FString Mapname = SessionSearch->SearchResults[SearchIdx].Session.SessionSettings.Settings.FindRef("TEST").Data.ToString();

					WidgetInstanceTextServerMapname->SetText(FText::FromString(Mapname));
					WidgetInstanceTextServerGamemode->SetText(FText::FromString(Gamemode));

					ServerList->AddChild(WidgetInstance);

					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Session Number: %d | Sessionname: %s "), SearchIdx + 1, *( SessionSearch->SearchResults[SearchIdx].Session.OwningUserName )));
				}
			}
		}
	}
}

bool UFarmionMainGameInstance::JoinSessionBool(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult & SearchResult) {
	// Return bool
	bool bSuccessful = false;

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if(OnlineSub) {
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if(Sessions.IsValid() && UserId.IsValid()) {
			// Set the Handle again
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			// Call the "JoinSession" Function with the passed "SearchResult". The "SessionSearch->SearchResults" can be used to get such a
			// "FOnlineSessionSearchResult" and pass it. Pretty straight forward!
			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);

		}
	}

	return bSuccessful;
}

void UFarmionMainGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>( Result )));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if(OnlineSub) {
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if(Sessions.IsValid()) {
			// Clear the Delegate again
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			// Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
			// This is something the Blueprint Node "Join Session" does automatically!
			APlayerController * const PlayerController = GetFirstLocalPlayerController();

			// We need a FString to use ClientTravel and we can let the SessionInterface contruct such a
			// String for us by giving him the SessionName and an empty String. We want to do this, because
			// Every OnlineSubsystem uses different TravelURLs
			FString TravelURL;

			if(PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL)) {
				// Finally call the ClientTravel. If you want, you could print the TravelURL to see
				// how it really looks like
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);

				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Travel URL:  %s"), *TravelURL));

			}
		}
	}
}

void UFarmionMainGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful) {
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if(OnlineSub) {
		// Get the SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if(Sessions.IsValid()) {
			// Clear the Delegate
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

			// If it was successful, we just load another level (could be a MainMenu!)
			if(bWasSuccessful) {
				UGameplayStatics::OpenLevel(GetWorld(), "Developers", true);
			}
		}
	}
}
