// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KioCheckpoint.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "KioCheckpointOverviewer.generated.h"

UCLASS()
class KIO_API AKioCheckpointOverviewer : public AActor {
	GENERATED_BODY()

public:
	AKioCheckpointOverviewer();

	UPROPERTY( BlueprintReadOnly, Category = "Checkpoints" ) TMap<int32, AKioCheckpoint*> CheckpointDictionary;

	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) bool bDebugMode = true;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) bool bDrawDebugCheckpointLines = true;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) float DebugCheckpointLineThickness = 5.0F;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) bool bDrawDebugCheckpointBox = true;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) float DebugCheckpointBoxThickness = 5.0F;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) float DebugTextSize = 1.0F;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) float DebugTextThickness = 5.0F;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) bool bRotateDebugTextToCamera = true;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) bool bDrawDebugText = true;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) bool bTextHasMaxDistance = true;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) bool bTextCapitalize = true;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) int32 DebugMaximumTextDistance = 5000;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) FColor DebugLineColor = FColor::Cyan;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) FColor DebugBoxColor = FColor::Cyan;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) FColor DebugTextColor = FColor::Cyan;

public:
	void AddCheckpoint( int32 CheckpointIndex, AKioCheckpoint* CheckpointRef );

	virtual bool ShouldTickIfViewportsOnly() const override;


protected:
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
		void TestFunction();

private:
	void BindDelegate( AKioCheckpoint* CheckpointRef );
	void DrawDebug();

};
