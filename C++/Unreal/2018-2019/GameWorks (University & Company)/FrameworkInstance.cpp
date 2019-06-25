// Copyright 2018 StrateGis Groep B.V. All Rights Reserved.
// Class UFrameworkInstance created by Jesse van Vliet

#include "FrameworkInstance.h"
#include "StrateGisGameWork.h"
#include "FOnlineSubSystemKey.h"
#include "OnlineSessionInterface.h"
#include "JsonReader.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "JsonObjectConverter.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "JsonSerializer.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UFrameworkInstance::UFrameworkInstance( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	Http = &FHttpModule::Get();

	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject( this, &UFrameworkInstance::OnCreateSessionComplete );
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject( this, &UFrameworkInstance::OnStartOnlineGameComplete );
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject( this, &UFrameworkInstance::OnFindSessionsComplete );
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject( this, &UFrameworkInstance::OnJoinSessionComplete );
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject( this, &UFrameworkInstance::OnDestroySessionComplete );
}


bool UFrameworkInstance::VictoryPC_GetMyIP_SendRequest()
{
	FHttpModule* Http = &FHttpModule::Get();

	if ( !Http )
	{
		return false;
	}

	if ( !Http->IsHttpEnabled() )
	{
		return false;
	}

	FString TargetHost = "http://api.ipify.org";
	TSharedRef < IHttpRequest > Request = Http->CreateRequest();
	Request->SetVerb( "GET" );
	Request->SetURL( TargetHost );
	Request->SetHeader( "User-Agent", "VictoryBPLibrary/1.0" );
	Request->SetHeader( "Content-Type", "text/html" );

	Request->OnProcessRequestComplete().BindUObject( this, &UFrameworkInstance::HTTPOnIpResponseReceived );
	if ( !Request->ProcessRequest() )
	{
		return false;
	}

	return true;
}

void UFrameworkInstance::HTTPOnIpResponseReceived( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful )
{
	FString ResponseStr = "UFrameworkInstance::HTTPOnIpResponseReceived>>> Connection Error";
	if ( bWasSuccessful )
	{
		ResponseStr = Response->GetContentAsString();
	}

	this->VictoryPC_GetMyIP_DataReceived( ResponseStr );
}

FModuleData UFrameworkInstance::SetModuleData( FModuleData ModuleData )
{
	CurrentModuleData = ModuleData;

	OnModuleChanged.Broadcast();

	return CurrentModuleData;
}

void UFrameworkInstance::ClearModule()
{
	SetModuleData( FModuleData() );
}

bool UFrameworkInstance::CreateSession( FName SessionName, bool IsLan, bool IsPresence, int32 PublicConnections, int32 PrivateConnections, bool AllowInvites, bool AllowJoinInProgress, bool ShouldAdvertise, bool AllowJoinViaPresence, bool AllowJoinViaPresenceFriendsOnly, FString MapName )
{
	FOnlineSubSystemKey* OnlineKey = FOnlineSubSystemKey().SubSystemValidator();

	if ( OnlineKey->ValidKey )
	{
		SessionSettings = MakeShareable( new FOnlineSessionSettings() );

		SessionSettings->bIsLANMatch = IsLan;
		SessionSettings->bUsesPresence = IsPresence;
		SessionSettings->NumPublicConnections = PublicConnections;
		SessionSettings->NumPrivateConnections = PrivateConnections;
		SessionSettings->bAllowInvites = AllowInvites;
		SessionSettings->bAllowJoinInProgress = AllowJoinInProgress;
		SessionSettings->bShouldAdvertise = ShouldAdvertise;
		SessionSettings->bAllowJoinViaPresence = AllowJoinViaPresence;
		SessionSettings->bAllowJoinViaPresenceFriendsOnly = AllowJoinViaPresenceFriendsOnly;

		SessionSettings->Set( SETTING_MAPNAME, MapName, EOnlineDataAdvertisementType::ViaOnlineService );

		OnCreateSessionCompleteDelegateHandle = OnlineKey->Sessions->AddOnCreateSessionCompleteDelegate_Handle( OnCreateSessionCompleteDelegate );

		return OnlineKey->Sessions->CreateSession( *OnlineKey->UniqueNetId, SessionName, *SessionSettings );

	}

	return false;
}

void UFrameworkInstance::OnCreateSessionComplete( FName SessionName, bool bWasSuccessful )
{
	FOnlineSubSystemKey* OnlineKey = FOnlineSubSystemKey().SubSystemValidator();

	if ( OnlineKey->ValidKey )
	{
		OnlineKey->Sessions->ClearOnCreateSessionCompleteDelegate_Handle( OnCreateSessionCompleteDelegateHandle );

		if ( bWasSuccessful )
		{
			OnStartSessionCompleteDelegateHandle = OnlineKey->Sessions->AddOnStartSessionCompleteDelegate_Handle( OnStartSessionCompleteDelegate );

			OnlineKey->Sessions->StartSession( SessionName );
		}
	}
}

void UFrameworkInstance::OnStartOnlineGameComplete( FName SessionName, bool bWasSuccessful )
{
	FOnlineSubSystemKey* OnlineKey = FOnlineSubSystemKey().SubSystemValidator();

	if ( OnlineKey->ValidKey )
	{
		OnlineKey->Sessions->ClearOnStartSessionCompleteDelegate_Handle( OnStartSessionCompleteDelegateHandle );
	}

	if ( bWasSuccessful )
	{
		UGameplayStatics::OpenLevel( GetWorld(), "Map_TestLevel", true, "listen" );
	}
}

void UFrameworkInstance::FindSessions( bool SearchLan, int32 MaxSearchResults, int32 PingBucketSize )
{
	FOnlineSubSystemKey* OnlineKey = FOnlineSubSystemKey().SubSystemValidator();

	if ( OnlineKey->ValidKey )
	{
		SessionSearch = MakeShareable( new FOnlineSessionSearch() );

		SessionSearch->bIsLanQuery = SearchLan;
		SessionSearch->MaxSearchResults = MaxSearchResults;
		SessionSearch->PingBucketSize = PingBucketSize;

		TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

		OnFindSessionsCompleteDelegateHandle = OnlineKey->Sessions->AddOnFindSessionsCompleteDelegate_Handle( OnFindSessionsCompleteDelegate );

		OnlineKey->Sessions->FindSessions( *OnlineKey->UniqueNetId, SearchSettingsRef );
	}
}


// TODO: Send to FOnlineSubSystemKey.h
void UFrameworkInstance::OnFindSessionsComplete( bool bWasSuccessful )
{
	FOnlineSubSystemKey* OnlineKey = FOnlineSubSystemKey().SubSystemValidator();

	if ( bWasSuccessful )
	{

		if ( OnlineKey->ValidKey )
		{
			OnlineKey->Sessions->ClearOnFindSessionsCompleteDelegate_Handle( OnFindSessionsCompleteDelegateHandle );

			if ( SessionSearch->SearchResults.Num() > 0 )
			{
				TArray<FBlueprintSessionResult> BlueprintSessionSearchResults;

				for ( uint8 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++ )
				{
					FBlueprintSessionResult NewResult = FBlueprintSessionResult();

					NewResult.OnlineResult = SessionSearch->SearchResults[SearchIdx];
					BlueprintSessionSearchResults.Add( NewResult );
				}

				OnFoundSessionDelegate.Broadcast( BlueprintSessionSearchResults );
			}
		}
	}
}

bool UFrameworkInstance::JoinSession( FName SessionName, FBlueprintSessionResult SearchResult )
{
	bool bSuccessful = false;

	FOnlineSubSystemKey* OnlineKey = FOnlineSubSystemKey().SubSystemValidator();

	if ( OnlineKey->ValidKey )
	{
		OnJoinSessionCompleteDelegateHandle = OnlineKey->Sessions->AddOnJoinSessionCompleteDelegate_Handle( OnJoinSessionCompleteDelegate );

		bSuccessful = OnlineKey->Sessions->JoinSession( *OnlineKey->UniqueNetId, SessionName, SearchResult.OnlineResult );

	}

	return bSuccessful;
}

void UFrameworkInstance::OnJoinSessionComplete( FName SessionName, EOnJoinSessionCompleteResult::Type Result )
{
	FOnlineSubSystemKey* OnlineKey = FOnlineSubSystemKey().SubSystemValidator();

	if ( OnlineKey->ValidKey )
	{
		OnlineKey->Sessions->ClearOnJoinSessionCompleteDelegate_Handle( OnJoinSessionCompleteDelegateHandle );

		APlayerController * const PlayerController = GetFirstLocalPlayerController();

		FString TravelURL;

		if ( PlayerController && OnlineKey->Sessions->GetResolvedConnectString( SessionName, TravelURL ) )
		{
			PlayerController->ClientTravel( TravelURL, ETravelType::TRAVEL_Absolute );
		}
	}
}


void UFrameworkInstance::DestroySession()
{
	FOnlineSubSystemKey* OnlineKey = FOnlineSubSystemKey().SubSystemValidator();

	if ( OnlineKey->ValidKey )
	{
		OnlineKey->Sessions->AddOnDestroySessionCompleteDelegate_Handle( OnDestroySessionCompleteDelegate );
		OnlineKey->Sessions->DestroySession( GameSessionName );
	}
}

void UFrameworkInstance::OnDestroySessionComplete( FName SessionName, bool bWasSuccessful )
{
	FOnlineSubSystemKey* OnlineKey = FOnlineSubSystemKey().SubSystemValidator();

	if ( OnlineKey->ValidKey )
	{
		OnlineKey->Sessions->ClearOnDestroySessionCompleteDelegate_Handle( OnDestroySessionCompleteDelegateHandle );

		if ( bWasSuccessful )
		{
			UGameplayStatics::OpenLevel( GetWorld(), "Map_MainMenu", true );
		}
	}
}

void UFrameworkInstance::DownloadModuleJson( FString Url )
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject( this, &UFrameworkInstance::ReceiveModuleJson );
	Request->SetURL( Url );
	Request->SetVerb( "GET" );
	Request->SetHeader( TEXT( "User-Agent" ), "X-UnrealEngine-Agent" );
	Request->SetHeader( "Content-Type", TEXT( "application/json" ) );
	Request->ProcessRequest();

}

void UFrameworkInstance::ReceiveModuleJson( FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful )
{
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create( Response->GetContentAsString() );
	CurrentModuleData = GetStructFromJsonString( Response );

	const UWorld* World = GetWorld();

	if ( World->GetGameState() )
	{
		AFrameworkGameState* FrameworkGameState = Cast<AFrameworkGameState>( World->GetGameState() );


		if ( FrameworkGameState != nullptr && FrameworkGameState->IsValidLowLevel() )
		{
			FrameworkGameState->CurrentModuleData = CurrentModuleData;
			OnModuleDownloadComplete.Broadcast( CurrentModuleData );
		}
	}
}

FModuleData UFrameworkInstance::GetStructFromJsonString( FHttpResponsePtr Response )
{
	FModuleData NewData;

	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<FModuleData>( JsonString, &NewData, 0, 0 );

	return NewData;
}

