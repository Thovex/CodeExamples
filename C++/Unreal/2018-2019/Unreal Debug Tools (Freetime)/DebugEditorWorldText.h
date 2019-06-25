// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#pragma once

#include "Kio.h"
#include "GameFramework/Actor.h"
#include "DebugEditorWorldText.generated.h"

UCLASS()
class KIO_API ADebugEditorWorldText : public AActor
{
	GENERATED_BODY()
	
public:	
	ADebugEditorWorldText();

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite ) USceneComponent* Transform;

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
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual bool ShouldTickIfViewportsOnly() const override;

private:
	void DrawDebug();


	
	
};
