// Copyright 2018 StrateGis Groep B.V. All Rights Reserved.
// Class AActorPlayerController created by Jesse van Vliet

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Framework/World/FMeasureInvestment.h"
#include "Framework/FrameworkGameState.h"
#include "Framework/ModuleDataAsset.h"
#include "ActorPlayerController.generated.h"

/**
 *
 */

class AMeasurePlot;
class ACenterActor;
class AActorPawn;

UCLASS()
class STRATEGISGAMEWORK_API AActorPlayerController : public APlayerController
{
	GENERATED_BODY()

#pragma region Construction
public:
	AActorPlayerController();

	UFUNCTION( BlueprintImplementableEvent )
		void EnablePlayer();

protected:
	virtual void PlayerTick( float DeltaTime );
	virtual void BeginPlay();

	void GetRequiredReferences();
	void SetupInputComponent();

	void InitPlayer();

private:
	AActorPawn * ActorPawn;
	AFrameworkGameState* FrameworkGameState;
#pragma endregion


#pragma region PlayerSettings
public:
	UPROPERTY( Replicated, EditAnywhere, BlueprintReadWrite )
		FString PlayerName;

	UPROPERTY( Replicated, EditAnywhere, BlueprintReadWrite )
		FString PlayerActor;

	UPROPERTY( ReplicatedUsing=OnRep_IsTurn, EditAnywhere, BlueprintReadWrite )
		bool bIsTurn = false;

	TMap<FString, float> GetGainsPerActor( int32 Cost, FMeasureInvestment MeasureInvestment );

	UFUNCTION()
	virtual void OnRep_IsTurn();

	UFUNCTION( BlueprintImplementableEvent )
		void ThisPlayersTurn();
	
	UFUNCTION( BlueprintCallable, Reliable, Server, WithValidation )
		void ServerEndTurnByIndex( int32 NewTurnIndex );

	FORCEINLINE bool ServerEndTurnByIndex_Validate(int32 NewTurnIndex ) { return true; };

	UFUNCTION( Reliable, Server, WithValidation )
		void ServerEndTurn();

	FORCEINLINE bool ServerEndTurn_Validate() { return true; };

	UFUNCTION( BlueprintCallable, Category = "Edit Player Data" )
		void SetPlayerName( FString NewPlayerName );

	UFUNCTION( BlueprintCallable, Category = "Edit Player Data" )
		bool VerifyPlayerName( FString PlayerNameToCheck );

	UFUNCTION( BlueprintCallable, Category = "Edit Player Data" )
		bool SetPlayerActor( FString NewActorName );

#pragma endregion PlayerSettings

#pragma region PlayerNetworking
	UFUNCTION( Reliable, Server, WithValidation )
		void ServerSetPlayerActor( const FString& NewActorName );

	UFUNCTION( Reliable, Server, WithValidation )
		void ServerSetPlayerName( AActorPlayerController* PlayerController, const FString& NewPlayerName );

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Replicated )
		int32 ActorScorePercentage;

	UFUNCTION( BlueprintCallable, Reliable, Server, WithValidation )
		void ServerSetPlayerScore( int32 NewScore );

#pragma endregion PlayerNetworking

#pragma region MeasureData
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		TSubclassOf<class UModuleDataAsset> ModuleData;
#pragma endregion MeasureData

#pragma region MeasureSelection

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		AMeasurePlot* SelectedPlot;

	UFUNCTION( BlueprintCallable )
		void SetMeasureTarget( AMeasurePlot* NewSelectedPlot );
#pragma endregion MeasureSelection

#pragma region Camera
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		UPrimitiveComponent* CameraFollowActor;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		AActor * CenterActor;

	UFUNCTION( BlueprintCallable )
		void SetCenterActor( ACenterActor* NewCenterActor );

	UFUNCTION( BlueprintCallable )
		void SetNewCameraLocation( FVector NewLocation, bool bHold, FTransform SceneComponentToGo );

	UFUNCTION( BlueprintCallable )
		void SetLocationCameraHold( UPrimitiveComponent* SelectedPrimitiveComponent );

	UFUNCTION( BlueprintCallable )
		void SetLocationCamera( FVector Location, bool bHold, const FTransform& SceneTransform );
#pragma endregion Camera


#pragma region Networking
	void LoopToApplyScores( int32 MeasureIndex, bool bNegative );

	UFUNCTION( Reliable, Server, WithValidation, BlueprintCallable )
		void ServerDestroyBuilding( AMeasurePlot* MeasurePlot, const FString& Actor );

	FORCEINLINE bool ServerDestroyBuilding_Validate( AMeasurePlot* MeasurePlot, const FString& Actor ) { return true; };

	UFUNCTION( Reliable, Server, WithValidation, BlueprintCallable )
		void ServerSetInvestment( AMeasurePlot* MeasurePlot, int32 InvestmentIndex, FMeasureInvestment NewInvestment );

	FORCEINLINE bool ServerSetInvestment_Validate( AMeasurePlot* MeasurePlot, int32 InvestmentIndex, FMeasureInvestment NewInvestment ) { return true; };

	UFUNCTION( Reliable, Server, WithValidation, BlueprintCallable )
		void ServerSetBuildingComplete( AMeasurePlot* MeasurePlot, FMeasureInvestment MeasureInvestment, const FString& MeasureOwner, bool bStartInit );

	FORCEINLINE bool ServerSetBuildingComplete_Validate( AMeasurePlot* MeasurePlot, FMeasureInvestment MeasureInvestment, const FString& MeasureOwner, bool bStartInit ) { return true; };

	UFUNCTION( Reliable, Server, WithValidation, BlueprintCallable )
		void ServerSetRequirementComplete( AMeasurePlot* MeasurePlot, int32 RequirementIndex, FMeasureInvestment MeasureInvestment );

	FORCEINLINE bool ServerSetRequirementComplete_Validate( AMeasurePlot* MeasurePlot, int32 RequirementIndex, FMeasureInvestment MeasureInvestment ) { return true; };
	
	UFUNCTION( Reliable, Server, WithValidation, BlueprintCallable )
		void ServerSetDestination( AMeasurePlot* MeasurePlot, const FString& NewDestination );

	FORCEINLINE bool ServerSetDestination_Validate( AMeasurePlot* MeasurePlot, const FString& NewDestination ) { return true; };

	UFUNCTION( Reliable, Server, WithValidation, BlueprintCallable )
		void ServerReturnInvestments( AMeasurePlot* MeasurePlot, const FString& Target, int32 TotalCost );

	FORCEINLINE bool ServerReturnInvestments_Validate( AMeasurePlot* MeasurePlot, const FString& Target, int32 TotalCost ) { return true; };

#pragma endregion Networking

};
