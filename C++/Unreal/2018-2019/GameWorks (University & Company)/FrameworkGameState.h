// Copyright 2018 StrateGis Groep B.V. All Rights Reserved.
// Class AFrameworkGameState created by Jesse van Vliet

#pragma once

#include "CoreMinimal.h"
#include "StrateGisGameWork.h"
#include "GameFramework/GameState.h"
#include "Framework/World/MeasurePlot.h"
#include "Framework/World/MeasureAesthetic.h"
#include "Module/FModuleData.h"
#include "FrameworkGameState.generated.h"

DECLARE_LOG_CATEGORY_EXTERN( GameStateLog, Log, All );

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FGameStarted );

USTRUCT( BlueprintType )
struct STRATEGISGAMEWORK_API FPlayerTurn
{
	GENERATED_USTRUCT_BODY()

	FPlayerTurn()
	{
		ActorName = "Struct Not Set";
		bIsActorTurn = false;
	}

	FPlayerTurn( FString _ActorName, bool _bIsActorTurn )
	{
		ActorName = _ActorName;
		bIsActorTurn = _bIsActorTurn;
	}

	UPROPERTY( BlueprintReadWrite, EditAnywhere )
		FString ActorName;

	UPROPERTY( BlueprintReadWrite, EditAnywhere )
		bool bIsActorTurn;
};

USTRUCT( BlueprintType )
struct STRATEGISGAMEWORK_API FActorScore
{
	GENERATED_USTRUCT_BODY()

	FActorScore()
	{
		ScoreName = "InvalidName";
	}

	FActorScore( FString _ScoreName, int32 _ScoreValue )
	{
		ScoreName = _ScoreName;
		ScoreValue = _ScoreValue;
	}

	UPROPERTY( BlueprintReadWrite, EditAnywhere )
		FString ScoreName;

	UPROPERTY( BlueprintReadWrite, EditAnywhere )
		int32 ScoreValue;
};


/**
 *
 */
UCLASS()
class STRATEGISGAMEWORK_API AFrameworkGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	AFrameworkGameState( const FObjectInitializer& ObjectInitializer );

	UPROPERTY( Replicated, BlueprintReadWrite, EditAnywhere )
		bool bIsTurnBasedOn = false;

	UPROPERTY( Replicated, BlueprintReadOnly, VisibleAnywhere )
		TArray<FString> ActorsPossesed;

	UPROPERTY( Replicated, BlueprintReadOnly )
		FModuleData CurrentModuleData;

	UPROPERTY( Replicated, BlueprintReadWrite, EditAnywhere )
		TArray<FActorScore> CurrentActorScores;

	UPROPERTY( Replicated, BlueprintReadWrite, EditAnywhere )
		bool bScoresSet = false;

	UFUNCTION( BlueprintCallable, Category = "Set Module Score" )
		void SetAddActorScoreByScoreName( FString ScoreName, int32 ScoreToAdd );

	UFUNCTION( BlueprintPure, Category = "Get Module Score" )
		int32 GetActorScoreByScoreName( FString ScoreName );

	UFUNCTION( BlueprintPure, Category = "Get Module Score" )
		int32 GetActorScoreIndexByScoreName( FString ScoreName );

	UFUNCTION( BlueprintPure, Category = "Get Module Data" )
		FModuleSettings ReturnSettings();

	UFUNCTION( BlueprintPure, Category = "Get Module Data" )
		FModuleGameplay ReturnGameplay();

	UFUNCTION( BlueprintPure, Category = "Get Module Data" )
		FModuleActors ReturnActors( int Index );

	UFUNCTION( BlueprintPure, Category = "Get Module Data" )
		FModuleActors ReturnActorsByString( FString ActorName );

	UFUNCTION( BlueprintPure, Category = "Get Module Data" )
		FModuleMeasures ReturnMeasure( int Index );

	UFUNCTION( BlueprintPure, Category = "Get Module Data" )
		TArray<FModuleActors> ReturnActorsArray();

	UFUNCTION( BlueprintPure, Category = "Get Module Data" )
		TArray<FModuleMeasures> ReturnMeasuresArray();

	UFUNCTION( BlueprintCallable, Reliable, Server, WithValidation )
		void ServerSetTurnBased( bool SetTurnbasedOn );

public:
	UPROPERTY( BlueprintAssignable, Category = "Framework Delegate" )
		FGameStarted OnGameStarted;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Replicated )
		bool bUsingTurnBasedGameplay = false;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Replicated )
		TArray<FPlayerTurn> PlayerTurnList;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Replicated )
		TArray<AActorPlayerController*> PlayerList;


protected:
	virtual void BeginPlay();

	void SetScores();
	void SetTurns();
	void SetActors();
	void SetExistingMeasures();

private:
	UFUNCTION( BlueprintCallable, Reliable, NetMulticast, WithValidation )
		void MulticastStartGame();

	FORCEINLINE bool MulticastStartGame_Validate() { return true; };

	UFUNCTION( BlueprintCallable, Reliable, Server, WithValidation )
		void ServerStartGame();

	FORCEINLINE bool ServerStartGame_Validate() { return true; };

};
