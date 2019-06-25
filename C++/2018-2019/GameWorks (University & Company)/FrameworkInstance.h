// Copyright 2018 StrateGis Groep B.V. All Rights Reserved.
// Class UFrameworkInstance created by Jesse van Vliet

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Module/FModuleData.h"
#include "UnrealNetwork.h"
#include "Online.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "FindSessionsCallbackProxy.h"
#include "FrameworkGameState.h"
#include "FrameworkInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FFoundSessionDelegate, const TArray<FBlueprintSessionResult>&, SessionSearchResult );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FModuleDataDelegate, FModuleData, DownloadedModuleData );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FModuleChangedDelegate );


/**
 *
 */
UCLASS( BlueprintType )
class STRATEGISGAMEWORK_API UFrameworkInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	/**
	* Constructor
	*/
	UFrameworkInstance( const FObjectInitializer& ObjectInitializer );

	/** This node relies on http://api.ipify.org, so if this node ever stops working, check out http://api.ipify.org.  Returns false if the operation could not occur because HTTP module was not loaded or unable to process request. */
	UFUNCTION( BlueprintCallable, Category = "Victory PC" )
		bool VictoryPC_GetMyIP_SendRequest();

	/** Implement this event to receive your IP once the request is processed! This requires that your computer has a live internet connection */
	UFUNCTION( BlueprintImplementableEvent, Category = "Victory PC" )
		void VictoryPC_GetMyIP_DataReceived( const FString& YourIP );

	void HTTPOnIpResponseReceived( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful );

	UFUNCTION( BlueprintCallable, Category = "Network|CPP" )
		virtual bool CreateSession( FName SessionName, bool IsLan, bool IsPresence, int32 PublicConnections, int32 PrivateConnections, bool AllowInvites, bool AllowJoinInProgress, bool ShouldAdvertise, bool AllowJoinViaPresence, bool AllowJoinViaPresenceFriendsOnly, FString MapName );

	UFUNCTION( BlueprintCallable, Category = "Network|CPP" )
		virtual void FindSessions( bool SearchLan, int32 MaxSearchResults, int32 PingBucketSize );

	UFUNCTION( BlueprintCallable, Category = "Network|CPP" )
		virtual bool JoinSession( FName SessionName, FBlueprintSessionResult SearchResult );

	UFUNCTION( BlueprintCallable, Category = "Network|CPP" )
		virtual void DestroySession();

	virtual bool JoinSession( ULocalPlayer* LocalPlayer, int32 SessionIndexInSearchResults ) { return false; }
	virtual bool JoinSession( ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult ) { return false; }

	/**
	* Called when OnFindSessionComplete has located all sessions and sent them to DisplaySessions
	*/
	UPROPERTY( BlueprintAssignable, Category = "Framework Delegate" )
		FFoundSessionDelegate OnFoundSessionDelegate;

	/**
	* Sets a new ModuleData based on parameter input
	*/
	UFUNCTION( BlueprintCallable, Category = "Module Data" )
		FModuleData SetModuleData( FModuleData ModuleData );

	/**
	* Console command to set module to nullptr. Use by opening console and typing "ClearModule"
	*/
	UFUNCTION( Exec, Category = "Console Commands" )
		void ClearModule();

	/**
	* Call the OnChangedModule event in Blueprint
	*/
	UPROPERTY( BlueprintAssignable, Category = "Framework Delegate" )
		FModuleChangedDelegate OnModuleChanged;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Module Data" )
		FModuleData CurrentModuleData;

	UFUNCTION( BlueprintCallable, Category = "Network|Downloading" )
		void DownloadModuleJson( FString Url );

	UPROPERTY( BlueprintAssignable, Category = "Framework Delegate" )
		FModuleDataDelegate OnModuleDownloadComplete;

	//virtual void OnConstruction(const FTransform& Transform) override;

private:

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	/**
	* Called when CreateSession is completed. Called by delegate
	*/
	virtual void OnCreateSessionComplete( FName SessionName, bool bWasSuccessful );

	/**
	* Called when OnCreateSessionComplete is completed. Called by delegate
	*/
	void OnStartOnlineGameComplete( FName SessionName, bool bWasSuccessful );

	/**
	* Called when FindSessions is completed. Called by delegate
	*/
	void OnFindSessionsComplete( bool bWasSuccessful );

	/**
	* Called when JoinSession is completed. Called by delegate
	*/
	void OnJoinSessionComplete( FName SessionName, EOnJoinSessionCompleteResult::Type Result );

	/**
	* Called when DestroySession is completed. Called by delegate
	*/
	virtual void OnDestroySessionComplete( FName SessionName, bool bWasSuccessful );

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

	FHttpModule * Http;

	void ReceiveModuleJson( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful );

	FModuleData GetStructFromJsonString( FHttpResponsePtr Response );

};



