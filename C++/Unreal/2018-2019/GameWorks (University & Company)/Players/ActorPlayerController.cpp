// Copyright 2018 StrateGis Groep B.V. All Rights Reserved.
// Class AActorPlayerController created by Jesse van Vliet

#include "ActorPlayerController.h"
#include "Gamemodes/FrameworkGameMode.h"
#include "ActorPawn.h"
#include "UnrealNetwork.h"
#include "Framework/World/MeasurePlot.h"
#include "Framework/Players/ActorPawn.h"
#include "Module/FModuleMeasures.h"
#include "Module/FModuleEffects.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "ConstructorHelpers.h"


AActorPlayerController::AActorPlayerController()
{
	//static ConstructorHelpers::FClassFinder<UModuleDataAsset> ModuleDataBP(TEXT("/Game/Blueprints/Module/Measures/BP_MeasureList"));

	//if (ModuleDataBP.Class != NULL)
	//{
	//	ModuleData = ModuleDataBP.Class;
	//}	
}


void AActorPlayerController::PlayerTick( float DeltaTime )
{
	Super::PlayerTick( DeltaTime );

	if ( CameraFollowActor )
	{
		SetLocationCamera( CameraFollowActor->GetComponentLocation(), true, FTransform() );
	}
}

void AActorPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GetRequiredReferences();
}

void AActorPlayerController::GetRequiredReferences()
{
	if ( GetPawn() != nullptr )
	{
		ActorPawn = Cast<AActorPawn>( GetPawn() );
	}

	const UWorld* World = GetWorld();

	if ( World )
	{
		if ( World->GetGameState() )
		{
			FrameworkGameState = Cast<AFrameworkGameState>( World->GetGameState() );
		}
	}
}

bool AActorPlayerController::SetPlayerActor( FString NewActorName )
{
	ServerSetPlayerActor( NewActorName );

	if ( this->PlayerActor == NewActorName )
	{
		EnablePlayer();
		return true;
	}

	return false;
}


void AActorPlayerController::SetCenterActor( ACenterActor* NewCenterActor )
{
	if ( NewCenterActor )
	{
		CenterActor = NewCenterActor;

		if ( GetPawn() != nullptr )
		{
			ActorPawn = Cast<AActorPawn>( GetPawn() );
		}

		if ( ActorPawn )
		{
			ActorPawn->CenterActor = NewCenterActor;
		}
	}
}

void AActorPlayerController::ServerSetPlayerScore_Implementation( int32 NewScore )
{
	ActorScorePercentage = NewScore;
}

bool AActorPlayerController::ServerSetPlayerScore_Validate( int32 NewScore )
{
	return true;
}

void AActorPlayerController::SetMeasureTarget( AMeasurePlot* NewSelectedPlot )
{
	if ( NewSelectedPlot )
	{
		SelectedPlot = NewSelectedPlot;
	}

}

void AActorPlayerController::SetNewCameraLocation( FVector NewLocation, bool bHold, FTransform SceneComponentToGo )
{
	UE_LOG( LogTemp, Warning, TEXT( "Using some random function that shouldn't be used" ) );
}

void AActorPlayerController::LoopToApplyScores( int32 MeasureIndex, bool bNegative )
{
	if ( FrameworkGameState )
	{
		TArray<FModuleEffects> ModuleEffects = FrameworkGameState->ReturnMeasure( MeasureIndex ).Effects;

		if ( ModuleEffects.Num() > 0 )
		{
			for ( int32 Index = 0; Index < ModuleEffects.Num(); Index++ )
			{
				if ( bNegative )
				{
					FrameworkGameState->SetAddActorScoreByScoreName( ModuleEffects[Index].Name, ModuleEffects[Index].Value *-1 );
				}
				else
				{
					FrameworkGameState->SetAddActorScoreByScoreName( ModuleEffects[Index].Name, ModuleEffects[Index].Value );
				}
			}
		}
	}
}

void AActorPlayerController::ServerSetDestination_Implementation( AMeasurePlot* MeasurePlot, const FString& NewDestination )
{
	if ( MeasurePlot )
	{
		if ( MeasurePlot->MeasureDestination != NewDestination )
		{
			if ( FrameworkGameState )
			{
				int8 Index = 0;
				for ( FMeasureInvestment MeasureInvest : MeasurePlot->MeasureInvestments )
				{
					FModuleMeasures MeasureToCheck = FrameworkGameState->ReturnMeasure( MeasureInvest.MeasureIndex );
					if ( !MeasureToCheck.Destinations.Contains( NewDestination ) )
					{
						ServerReturnInvestments( MeasurePlot, MeasureInvest.ActorName, MeasureToCheck.Cost );

						FMeasureInvestment NewInvestment = FMeasureInvestment();
						NewInvestment.ActorName = MeasureInvest.ActorName;
						NewInvestment.bInUse = false;
						NewInvestment.MeasureIndex = 0;
						NewInvestment.ActorInvestedMap = MeasurePlot->GetEmptyInvestmentMap();

						ServerSetInvestment( MeasurePlot, Index, NewInvestment );
					}

					Index++;
				}

				MeasurePlot->MeasureDestination = NewDestination;

				if ( MeasurePlot->bIsInUse )
				{
					if ( MeasurePlot->AestheticPlot )
					{


						bool bIsAllowed = false;

						FModuleMeasures Measure = FrameworkGameState->ReturnMeasure( MeasurePlot->AestheticPlot->MeasureIndex );
						for ( FString MeasureDestination : Measure.Destinations )
						{
							if ( MeasureDestination == NewDestination )
							{
								bIsAllowed = true;
							}
						}

						if ( !bIsAllowed )
						{
							ServerDestroyBuilding( MeasurePlot, MeasurePlot->OwningActor );
						}

					}
				}
			}
		}
	}
}

void AActorPlayerController::ServerReturnInvestments_Implementation( AMeasurePlot* MeasurePlot, const FString& Target, int32 TotalCost )
{
	if ( MeasurePlot )
	{
		int32 Index;

		FMeasureInvestment MeasureInvestment = MeasurePlot->GetInvestmentByActorName( Target, Index );

		const UWorld* World = GetWorld();

		if ( World )
		{
			AFrameworkGameMode* GameMode = Cast<AFrameworkGameMode>( World->GetAuthGameMode() );

			if ( GameMode && GameMode->IsValidLowLevel() )
			{
				for ( FInvestmentMap Investment : MeasureInvestment.ActorInvestedMap )
				{
					for ( AActorPlayerController* Player : GameMode->PlayerList )
					{
						if (Player->PlayerActor == Investment.ActorInvested ) {
							if ( Player->GetPawn() )
							{
								if ( TotalCost < 0 )
								{

									Cast<AActorPawn>( Player->GetPawn() )->ServerSetCapitalSub( ( Investment.PercentInvested * 0.01f * TotalCost ) );
								}
								else
								{
									//Cast<AActorPawn>( Player->GetPawn() )->ServerSetCapitalSub( ( Investment.PercentInvested * 0.01f * TotalCost ) );
								}
							}
						}
					}
				}
			}
		}
	}
}

void AActorPlayerController::ServerDestroyBuilding_Implementation( AMeasurePlot* MeasurePlot, const FString& Actor )
{
	if ( MeasurePlot )
	{
		if ( MeasurePlot->AestheticPlot )
		{
			LoopToApplyScores( MeasurePlot->AestheticPlot->MeasureIndex, true );

			int32 Index;
			FMeasureInvestment MeasureInvestment = MeasurePlot->GetInvestmentByActorName( Actor, Index );

			if ( FrameworkGameState )
			{
				FModuleMeasures ModuleMeasure = FrameworkGameState->ReturnMeasure( MeasureInvestment.MeasureIndex );
				ServerReturnInvestments( MeasurePlot, Actor, ModuleMeasure.Cost );
			}

			FMeasureInvestment NewInvestment = FMeasureInvestment();
			NewInvestment.MeasureIndex = 0;
			NewInvestment.bInUse = false;
			NewInvestment.ActorName = MeasureInvestment.ActorName;
			NewInvestment.ActorInvestedMap = MeasurePlot->GetEmptyInvestmentMap();

			MeasurePlot->OwningActor = "";

			MeasurePlot->SetInvestmentByActorName( Actor, NewInvestment );

			MeasurePlot->AestheticPlot->DestroyAesthetic();
			MeasurePlot->AestheticPlot = nullptr;

			MeasurePlot->OnRep_SetAestheticPlot();
		}
	}
}

void AActorPlayerController::ServerSetBuildingComplete_Implementation( AMeasurePlot* MeasurePlot, FMeasureInvestment MeasureInvestment, const FString& MeasureOwner, bool bStartInit )
{
	if ( MeasurePlot )
	{
		MeasurePlot->OwningActor = MeasureOwner;

		LoopToApplyScores( MeasureInvestment.MeasureIndex, false );

		UWorld* World = GetWorld();

		if ( World )
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Need to seriously find a way to find the Module data if there's none set. The constructor commented solution makes the
			// engine stop loading at 72%. Your only solution right now is to go to BP_ActorPlayerController and set Module Data to the BP_MeasureList.
			// It's actually better to crash the engine so you can fix it right away. Because a nullcheck here will make YOU sweat (right before your deadline).

			// Yes the error is down here at this Line + 1, The solution is up there, Four lines back.
			if ( ModuleData.GetDefaultObject()->MeasureList[MeasureInvestment.MeasureIndex] )
			{
				AMeasureAesthetic* NewMeasureAesthetic = World->SpawnActor<AMeasureAesthetic>(
					ModuleData.GetDefaultObject()->MeasureList[MeasureInvestment.MeasureIndex], 
					MeasurePlot->TransformSceneComponent->GetComponentTransform(),
					Params 
				);

				if ( bStartInit )
				{
					NewMeasureAesthetic->bStartInitialized = true;
				}

				if ( FrameworkGameState )
				{
					FModuleMeasures ModuleMeasure = FrameworkGameState->ReturnMeasure( MeasureInvestment.MeasureIndex );

					if ( ModuleMeasure.Cost > 0 )
					{
						TMap<FString, float> Gains = GetGainsPerActor( ModuleMeasure.Cost, MeasureInvestment );
						const UWorld* World = GetWorld();

						if ( World )
						{
							AFrameworkGameMode* GameMode = Cast<AFrameworkGameMode>( World->GetAuthGameMode() );

							if ( GameMode && GameMode->IsValidLowLevel() )
							{
								for ( AActorPlayerController* Player : GameMode->PlayerList )
								{
									for ( auto& Elem : Gains )
									{
										if ( Elem.Key == Player->PlayerActor )
										{
											if ( Player->GetPawn() )
											{												
												Cast<AActorPawn>( Player->GetPawn() )->ServerSetCapitalAdd( Elem.Value );
											}
										}
									}
								}
							}
						}
					}
				}

				NewMeasureAesthetic->MeasureParent = MeasurePlot;
				NewMeasureAesthetic->MeasureIndex = MeasureInvestment.MeasureIndex;

				MeasurePlot->AestheticPlot = NewMeasureAesthetic;
				MeasurePlot->OnRep_SetAestheticPlot();
			}
			else
			{
				UE_LOG( LogTemp, Warning, TEXT( "Missing Module Data in ActorPlayerController!" ) );
			}
		}
	}
}

TMap<FString, float> AActorPlayerController::GetGainsPerActor(int32 Cost, FMeasureInvestment MeasureInvestment)
{
	TMap<FString, float> GainsPerActor;

	for ( FInvestmentMap Investment : MeasureInvestment.ActorInvestedMap )
	{
		float ActorGains = Cost / 100 * Investment.PercentInvested;
		GainsPerActor.Add( Investment.ActorInvested, ActorGains );
	}

	return GainsPerActor;
}

void AActorPlayerController::ServerSetInvestment_Implementation( AMeasurePlot* MeasurePlot, int32 InvestmentIndex, FMeasureInvestment NewInvestment )
{
	
	if ( MeasurePlot )
	{
		for ( FInvestmentMap InvMap : NewInvestment.ActorInvestedMap )
		{
			UE_LOG( LogTemp, Warning, TEXT( "%s : %f"), *InvMap.ActorInvested, InvMap.PercentInvested );

		}

		MeasurePlot->MeasureInvestments[InvestmentIndex] = NewInvestment;
		MeasurePlot->UpdateInvestment( MeasurePlot->MeasureInvestments );
	}
}

void AActorPlayerController::OnRep_IsTurn()
{
	if ( FrameworkGameState )
	{
		if ( !FrameworkGameState->bIsTurnBasedOn ) return;
	}

	if ( bIsTurn )
	{
		if ( IsOwnedBy( UGameplayStatics::GetPlayerController( this, 0 ) ) )
		{
			ThisPlayersTurn();
		}
	}
}

void AActorPlayerController::ServerEndTurnByIndex_Implementation(int32 NewTurnIndex)
{
	FString NotifyActor = "";

	if ( FrameworkGameState )
	{
		for ( uint8 Index = 0; Index < FrameworkGameState->PlayerTurnList.Num(); Index++ )
		{
			if ( FrameworkGameState->PlayerTurnList[Index].bIsActorTurn )
			{
				FrameworkGameState->PlayerTurnList[Index].bIsActorTurn = false;
			}
		}

		FrameworkGameState->PlayerTurnList[NewTurnIndex].bIsActorTurn = true;
		NotifyActor = FrameworkGameState->PlayerTurnList[NewTurnIndex].ActorName;
	}

	const UWorld* World = GetWorld();

	if ( World )
	{
		AFrameworkGameMode* GameMode = Cast<AFrameworkGameMode>( World->GetAuthGameMode() );

		if ( GameMode && GameMode->IsValidLowLevel() )
		{
			for ( AActorPlayerController* Player : GameMode->PlayerList )
			{
				if ( Player->PlayerActor == NotifyActor )
				{
					Player->bIsTurn = true;
				}
				else
				{
					Player->bIsTurn = false;
				}
				Player->OnRep_IsTurn();
			}
		}
	}

}

void AActorPlayerController::ServerEndTurn_Implementation()
{
	FString NotifyActor = "";

	if ( FrameworkGameState )
	{
		for ( uint8 Index = 0; Index < FrameworkGameState->PlayerTurnList.Num(); Index++ )
		{
			if ( FrameworkGameState->PlayerTurnList[Index].bIsActorTurn )
			{
				FrameworkGameState->PlayerTurnList[Index].bIsActorTurn = false;

				if ( Index + 1 < FrameworkGameState->PlayerTurnList.Num() )
				{
					FrameworkGameState->PlayerTurnList[Index + 1].bIsActorTurn = true;
					NotifyActor = FrameworkGameState->PlayerTurnList[Index + 1].ActorName;
					break;
				}
				else
				{
					FrameworkGameState->PlayerTurnList[0].bIsActorTurn = true;
					NotifyActor = FrameworkGameState->PlayerTurnList[0].ActorName;
					break;
				}
			}
		}
	}

	const UWorld* World = GetWorld();

	if ( World )
	{
		AFrameworkGameMode* GameMode = Cast<AFrameworkGameMode>( World->GetAuthGameMode() );

		if ( GameMode && GameMode->IsValidLowLevel() )
		{
			for ( AActorPlayerController* Player : GameMode->PlayerList )
			{
				if ( Player->PlayerActor == NotifyActor )
				{
					Player->bIsTurn = true;
				}
				else
				{
					Player->bIsTurn = false;
				}
				Player->OnRep_IsTurn();
			}
		}
	}
}

void AActorPlayerController::SetPlayerName( FString NewPlayerName )
{
	ServerSetPlayerName( this, NewPlayerName );
}

bool AActorPlayerController::VerifyPlayerName( FString PlayerNameToCheck )
{
	if ( this->PlayerName == PlayerNameToCheck )
	{
		return true;
	}


	return false;
}

bool AActorPlayerController::ServerSetPlayerActor_Validate( const FString& NewActorName )
{
	return true;
}

void AActorPlayerController::ServerSetPlayerActor_Implementation( const FString& NewActorName )
{
	PlayerActor = NewActorName;
}

bool AActorPlayerController::ServerSetPlayerName_Validate( AActorPlayerController* PlayerController, const FString& NewPlayerName )
{
	return true;
}

void AActorPlayerController::ServerSetPlayerName_Implementation( AActorPlayerController* PlayerController, const FString& NewPlayerName )
{
	UWorld* World = GetWorld();

	if ( World != nullptr && World->IsValidLowLevel() )
	{
		AFrameworkGameMode* GameModeBase = Cast<AFrameworkGameMode>( World->GetAuthGameMode() );

		if ( GameModeBase != nullptr && GameModeBase->IsValidLowLevel() )
		{
			for ( AActorPlayerController* GamemodePlayer : GameModeBase->PlayerList )
			{
				if ( GamemodePlayer->PlayerName == NewPlayerName )
				{
					return;
				}
			}
			PlayerController->PlayerName = NewPlayerName;
		}
	}
}

void AActorPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AActorPlayerController::ServerSetRequirementComplete_Implementation( AMeasurePlot* MeasurePlot, int32 RequirementIndex, FMeasureInvestment MeasureInvestment )
{
	MeasurePlot->HasRequirements.Add( RequirementIndex );

	LoopToApplyScores( MeasureInvestment.MeasureIndex, false );

	MeasurePlot->MakeNewMeasureInvestments();

	UWorld* World = GetWorld();

	if ( World )
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Need to seriously find a way to find the Module data if there's none set. The constructor commented solution makes the
		// engine stop loading at 72%. Your only solution right now is to go to BP_ActorPlayerController and set Module Data to the BP_MeasureList.
		// It's actually better to crash the engine so you can fix it right away. Because a nullcheck here will make YOU sweat (right before your deadline).

		// Yes the error is down here at this Line + 1, The solution is up there, Four lines back.
		if ( ModuleData.GetDefaultObject()->MeasureList[MeasureInvestment.MeasureIndex] )
		{
			AMeasureAesthetic* NewMeasureAesthetic = World->SpawnActor<AMeasureAesthetic>(
				ModuleData.GetDefaultObject()->MeasureList[MeasureInvestment.MeasureIndex],
				MeasurePlot->TransformSceneComponent->GetComponentTransform(),
				Params
				);

		}
	}
}

void AActorPlayerController::SetLocationCameraHold( UPrimitiveComponent* SelectedPrimitiveComponent )
{
	CameraFollowActor = SelectedPrimitiveComponent;
	SelectedPlot = nullptr;
}

void AActorPlayerController::SetLocationCamera( FVector Location, bool bHold, const FTransform& SceneTransform )
{
	if ( bHold )
	{
		CameraFollowActor = nullptr;
	}

	FVector Origin = FVector::ZeroVector;
	FVector BoxExtent = FVector::ZeroVector;

	if ( SelectedPlot )
	{
		if ( ActorPawn )
		{
			if ( ActorPawn->CenterActor )
			{
				if ( SelectedPlot->AestheticPlot )
				{
					SelectedPlot->AestheticPlot->GetActorBounds( true, Origin, BoxExtent );
				}

				if ( SelectedPlot->bIsInUse )
				{
					Location.Z += BoxExtent.Z;
				}
				else
				{
					Location.Z += 1500;
				}

				ActorPawn->CenterActor->LerpPosition = Location;
			}
		}
	}

	if ( !bHold )
	{
		ActorPawn->SetLerpPositionRotation( SceneTransform.GetLocation(), SceneTransform.GetRotation() );
	}

}

void AActorPlayerController::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	DOREPLIFETIME( AActorPlayerController, PlayerName );
	DOREPLIFETIME( AActorPlayerController, PlayerActor );
	DOREPLIFETIME( AActorPlayerController, bIsTurn );
	DOREPLIFETIME( AActorPlayerController, ActorScorePercentage );

}
