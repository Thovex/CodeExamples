// Copyright 2018 StrateGis Groep B.V. All Rights Reserved.
// Class AFrameworkGameState created by Jesse van Vliet

#include "FrameworkGameState.h"
#include "FrameworkInstance.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Framework/World/MeasureAesthetic.h"
#include "Framework/World/MeasurePlot.h"
#include "Framework/Players/ActorPlayerController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Framework/Players/ActorPlayerController.h"
#include "UnrealNetwork.h"
#include "Gamemodes/FrameworkGameMode.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY( GameStateLog );

AFrameworkGameState::AFrameworkGameState( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{

}

void AFrameworkGameState::BeginPlay()
{
	Super::BeginPlay();

}

void AFrameworkGameState::SetScores()
{
	if ( ROLE_Authority )
	{
		for ( int8 Index = 0; Index < CurrentModuleData.Gameplay.Effects.Num(); Index++ )
		{
			CurrentActorScores.Add( FActorScore( CurrentModuleData.Gameplay.Effects[Index], CurrentModuleData.Gameplay.EffectsInitial[Index] ) );
		}

		bScoresSet = true;
	}
}

void AFrameworkGameState::SetActors()
{
	if ( ROLE_Authority )
	{
		const UWorld* World = GetWorld();

		if ( World )
		{
			AFrameworkGameMode* GameMode = Cast<AFrameworkGameMode>( World->GetAuthGameMode() );

			if ( GameMode && GameMode->IsValidLowLevel() )
			{
				for ( AActorPlayerController* Player : GameMode->PlayerList) {
					if ( Player->PlayerActor != "Gamemanager" && Player->PlayerActor != "Spectator" )
					{
						ActorsPossesed.Add( Player->PlayerActor );
					}
				}
				PlayerList = GameMode->PlayerList;
			}
		}

		SetExistingMeasures();

	}

}

void AFrameworkGameState::SetExistingMeasures()
{
	if ( ROLE_Authority )
	{
		TArray<AMeasurePlot*> MeasurePlotsInWorld;

		for ( TActorIterator<AMeasurePlot> ActorItr( GetWorld() ); ActorItr; ++ActorItr )
		{
			MeasurePlotsInWorld.Add( *ActorItr );
		}


		FModuleGameplay Gameplay = ReturnGameplay();

		int32 Index = 0;


		AActorPlayerController* PlayerController = nullptr;

		const UWorld* World = GetWorld();

		if ( World )
		{
			AFrameworkGameMode* GameMode = Cast<AFrameworkGameMode>( World->GetAuthGameMode() );

			if ( GameMode && GameMode->IsValidLowLevel() )
			{
				for ( AActorPlayerController* Player : PlayerList )
				{
					if ( Player )
					{
						if ( Player->PlayerActor != "Spectator" && Player->PlayerActor != "Gamemanager" )
						{
							PlayerController = Player;
							UE_LOG( LogTemp, Warning, TEXT( "%s" ), *Player->PlayerActor );
							break;
						}
					}
				}
			}
		}

		if ( PlayerController != nullptr )
		{
			for ( int32 MeasureIndex : Gameplay.StartMeasures )
			{
				if ( MeasureIndex > 0 )
				{
					AMeasurePlot* SelectedPlot = nullptr;

					FString PlotNamePicked = Gameplay.Plotnames[Index];

					for ( AMeasurePlot* Plot : MeasurePlotsInWorld )
					{
						if ( Plot->PlotNameText.ToString() == PlotNamePicked )
						{
							SelectedPlot = Plot;
						}
					}

					FModuleMeasures MeasureToBuild = ReturnMeasure( MeasureIndex );
					FModuleActors OwningActor = ReturnActorsByString( PlayerController->PlayerActor );

					if ( SelectedPlot )
					{
						FMeasureInvestment MeasureInvestment = FMeasureInvestment();
						MeasureInvestment.ActorInvestedMap = SelectedPlot->GetEmptyInvestmentMap();
						MeasureInvestment.ActorName = OwningActor.Name;
						MeasureInvestment.bInUse = true;
						MeasureInvestment.MeasureIndex = MeasureIndex;

						SelectedPlot->OwningActor = OwningActor.Name;

						for ( int32 i = 0; i < MeasureInvestment.ActorInvestedMap.Num(); i++ )
						{
							if ( MeasureInvestment.ActorInvestedMap[i].ActorInvested == OwningActor.Name )
							{
								MeasureInvestment.ActorInvestedMap[i].PercentInvested = 100;

							}
						}

						for ( int32 i = 0; i < SelectedPlot->MeasureInvestments.Num(); i++ )
						{
							if ( SelectedPlot->MeasureInvestments[i].ActorName == OwningActor.Name )
							{
								SelectedPlot->MeasureInvestments[i] = MeasureInvestment;
							}
						}


						SelectedPlot->OnRep_SetMeasureInvestment();

						PlayerController->ServerSetBuildingComplete( SelectedPlot, MeasureInvestment, OwningActor.Name, true );
					}
				}

				Index++;
			}
		}
	}
}

void AFrameworkGameState::SetTurns()
{
	if ( ROLE_Authority )
	{
		if ( ReturnGameplay().Turnbased )
		{
			bIsTurnBasedOn = true;
		}

		const UWorld* World = GetWorld();

		if ( World )
		{
			AFrameworkGameMode* GameMode = Cast<AFrameworkGameMode>( World->GetAuthGameMode() );

			if ( GameMode && GameMode->IsValidLowLevel() )
			{
				for ( AActorPlayerController* Player : GameMode->PlayerList )
				{
					if ( Player )
					{
						if ( Player->PlayerActor != "Spectator" && Player->PlayerActor != "Gamemanager" )
						{
							PlayerTurnList.Add( FPlayerTurn( Player->PlayerActor, false ) );
						}
					}
				}

				if ( PlayerTurnList.Num() > 0 )
				{
					uint8 RandomActorIndex = FMath::RandRange( 0, PlayerTurnList.Num() - 1 );
					PlayerTurnList[RandomActorIndex].bIsActorTurn = true;

					Cast<AActorPlayerController>( UGameplayStatics::GetPlayerController( World, 0 ) )->ServerEndTurn();
				}
				
			}
		}
	}
}



void AFrameworkGameState::MulticastStartGame_Implementation()
{
	OnGameStarted.Broadcast();
}

void AFrameworkGameState::ServerStartGame_Implementation()
{
	SetActors();
	SetScores();
	SetTurns();
}

void AFrameworkGameState::SetAddActorScoreByScoreName( FString ScoreName, int32 ScoreToAdd )
{
	for ( int32 Index = 0; Index < CurrentActorScores.Num(); Index++ )
	{
		if ( CurrentActorScores[Index].ScoreName == ScoreName )
		{
			CurrentActorScores[Index].ScoreValue += ScoreToAdd;
		}
	}
}


int32 AFrameworkGameState::GetActorScoreByScoreName( FString ScoreName )
{
	for ( int32 Index = 0; Index < CurrentActorScores.Num(); Index++ )
	{
		if ( CurrentActorScores[Index].ScoreName == ScoreName )
		{
			return CurrentActorScores[Index].ScoreValue;
		}
	}

	return -1;
}

int32 AFrameworkGameState::GetActorScoreIndexByScoreName( FString ScoreName )
{
	for ( int32 Index = 0; Index < CurrentActorScores.Num(); Index++ )
	{
		if ( CurrentActorScores[Index].ScoreName == ScoreName )
		{
			return Index;
		}
	}

	UE_LOG( GameStateLog, Warning, TEXT( "Can't get actor score index by score name... returning -1" ) );
	return -1;
}

FModuleSettings AFrameworkGameState::ReturnSettings()
{
	FModuleSettings Settings = FModuleSettings();

	Settings = CurrentModuleData.Settings;

	return Settings;
}

FModuleGameplay AFrameworkGameState::ReturnGameplay()
{
	FModuleGameplay Gameplay = FModuleGameplay();

	Gameplay = CurrentModuleData.Gameplay;

	return Gameplay;
}

FModuleActors AFrameworkGameState::ReturnActors( int Index )
{
	FModuleActors Actors = FModuleActors();

	if ( CurrentModuleData.Actors.IsValidIndex( Index ) )
	{
		Actors = CurrentModuleData.Actors[Index];
		return Actors;
	}

	UE_LOG( GameStateLog, Warning, TEXT( "Going over or below measures Array using ReturnActors" ) );
	return Actors;
}

FModuleActors AFrameworkGameState::ReturnActorsByString( FString ActorName )
{
	FModuleActors Actors = FModuleActors();

	for ( int32 i = 0; i < CurrentModuleData.Actors.Num(); i++ )
	{
		if ( CurrentModuleData.Actors[i].Name == ActorName )
		{
			Actors = CurrentModuleData.Actors[i];
			return Actors;
		}
	}

	UE_LOG( GameStateLog, Warning, TEXT( "Actor by string not found." ) );
	return Actors;
}

FModuleMeasures AFrameworkGameState::ReturnMeasure( int Index )
{
	FModuleMeasures Measure = FModuleMeasures();

	if ( CurrentModuleData.Measures.IsValidIndex( Index ) )
	{
		Measure = CurrentModuleData.Measures[Index];
		return Measure;
	}

	UE_LOG( GameStateLog, Warning, TEXT( "Going over or below measures Array using ReturnMeasure" ) );
	return Measure;
}


TArray<FModuleActors> AFrameworkGameState::ReturnActorsArray()
{
	TArray<FModuleActors> ActorsArray;

	if ( CurrentModuleData.Actors.Num() > 0 )
	{
		ActorsArray = CurrentModuleData.Actors;
		return ActorsArray;
	}
	UE_LOG( GameStateLog, Warning, TEXT( "Actors array doesn't contain values in ReturnActorsArray" ) );
	return ActorsArray;
}

TArray<FModuleMeasures> AFrameworkGameState::ReturnMeasuresArray()
{
	TArray<FModuleMeasures> MeasuresArray;

	if ( CurrentModuleData.Measures.Num() > 0 )
	{
		MeasuresArray = CurrentModuleData.Measures;

		if ( MeasuresArray.IsValidIndex( 0 ) )
		{
			MeasuresArray.RemoveAt( 0, 1, true );
		}

		return MeasuresArray;
	}

	UE_LOG( GameStateLog, Warning, TEXT( "Measures array doesn't contain values in ReturnMeasuresArray" ) );
	return MeasuresArray;
}


void AFrameworkGameState::ServerSetTurnBased_Implementation( bool SetTurnbasedOn )
{
	bIsTurnBasedOn = SetTurnbasedOn;
}

bool AFrameworkGameState::ServerSetTurnBased_Validate( bool SetTurnbasedOn )
{
	return true;
}

void AFrameworkGameState::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AFrameworkGameState, CurrentActorScores );
	DOREPLIFETIME( AFrameworkGameState, CurrentModuleData );

	DOREPLIFETIME( AFrameworkGameState, bUsingTurnBasedGameplay );
	DOREPLIFETIME( AFrameworkGameState, PlayerTurnList );

	DOREPLIFETIME( AFrameworkGameState, ActorsPossesed );

	DOREPLIFETIME( AFrameworkGameState, bIsTurnBasedOn );
	DOREPLIFETIME( AFrameworkGameState, bScoresSet );

	DOREPLIFETIME( AFrameworkGameState, PlayerList );
}
