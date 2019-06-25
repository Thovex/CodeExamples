// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#pragma once

#include "Kio.h"
#include "Debug/DebugEditorTickingComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "DebugEditorLineCompToComp.generated.h"

/**
 *
 */
UCLASS( ClassGroup = ( Debug ), meta = ( BlueprintSpawnableComponent ), BlueprintType, Blueprintable )
class KIO_API UDebugEditorLineCompToComp : public UDebugEditorTickingComponent {
	GENERATED_BODY()

public:
	UDebugEditorLineCompToComp();

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Debug Line" )
		FName StartComponentName;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Debug Line" )
		FName EndComponentName;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Debug Line" )
		FColor DebugLineColor = FColor::Red;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Debug Line" )
		float DebugLineThickness = 5.0F;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	#if WITH_EDITOR
	virtual void PostEditChangeProperty( FPropertyChangedEvent& PropertyChangedEvent ) override;
	#endif

private:
	void FetchComponents();

	int8 Retries = 0;

	USceneComponent* StartComponent;
	USceneComponent* EndComponent;


};
