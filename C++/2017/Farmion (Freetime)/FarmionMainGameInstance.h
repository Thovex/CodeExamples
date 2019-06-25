// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FarmionMacros.h"
#include "Online.h"
#include "FindSessionsCallbackProxy.h"
#include "Widgets/ServerBrowserItem.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "FarmionMainGameInstance.generated.h"

/**
 *
 */
UCLASS()
class FARMION_API UFarmionMainGameInstance : public UGameInstance {
	GENERATED_BODY()

public:

	UFarmionMainGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Farmion Networking")
		void StartOnlineGame(bool bIsLan, bool bIsPresence, int32 NumPlayers, FName ServerName, FName MapName, FName GamemodeName);

	UFUNCTION(BlueprintCallable, Category = "Farmion Networking")
		void FindOnlineGames(bool bIsLan, bool bIsPresence, UClass* UserWidgetClass, UScrollBox* ScrollBox);

	UFUNCTION(BlueprintCallable, Category = "Farmion Networking")
		void JoinOnlineGame(FBlueprintSessionResult BlueprintSearchResult);

	UFUNCTION(BlueprintCallable, Category = "Farmion Networking")
		void DestroySessionAndLeaveGame();

private:
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	bool HostSessionBool(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, FName ServerName, FName MapName, FName GamemodeName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);
	bool JoinSessionBool(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	UScrollBox* ServerList;
	UClass* WidgetInstanceClass;


public:
	FName CreatedSessionPlayerName;
	FName CreatedSessionName;
	bool CreatedGame;
};
