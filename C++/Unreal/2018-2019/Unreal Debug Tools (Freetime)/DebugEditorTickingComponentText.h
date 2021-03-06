// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#pragma once

#include "Kio.h"
#include "Debug/DebugEditorTickingComponent.h"
#include "DebugEditorTickingComponentText.generated.h"

/**
 *
 */
UCLASS( ClassGroup = ( Debug ), meta = ( BlueprintSpawnableComponent ), BlueprintType, Blueprintable )
class KIO_API UDebugEditorTickingComponentText : public UDebugEditorTickingComponent {
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Debug" ) FVector DisplayOffset;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Debug" ) FString DisplayText;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) bool bDebugMode = true;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) float DebugTextSize = 1.0F;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) float DebugTextThickness = 5.0F;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) bool bRotateDebugTextToCamera = true;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) bool bTextHasMaxDistance = true;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) bool bTextCapitalize = true;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) int32 DebugMaximumTextDistance = 5000;
	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Debug" ) FColor DebugTextColor = FColor::Cyan;

protected:
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void DrawDebug();

};
