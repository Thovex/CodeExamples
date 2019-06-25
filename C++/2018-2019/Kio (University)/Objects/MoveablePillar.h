// Copyright 2018-2019 Prickly, All Rights Reserved.

#pragma once

#include "Kio.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Outlineable.h"
#include "Interfaces/Useable.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "MoveablePillar.generated.h"

class AKioCharacter;
class AKioPlayerController;

UENUM( BlueprintType )
enum class EMoveablePillarDirections : uint8 {
	Pillar_Forward                  UMETA( "Forward" ),
	Pillar_Right                    UMETA( "Right" ),
	Pillar_Backward                 UMETA( "Backward" ),
	Pillar_Left                     UMETA( "Left" ),
};

UCLASS()
class KIO_API AMoveablePillar : public AActor, public IOutlineable, public IUseable
{
	GENERATED_BODY()
	
public:	
	AMoveablePillar();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) 
		float DisplayTime = .5F;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly ) 
		float CurrentDisplayTime = 0.F;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		TMap<EMoveablePillarDirections, UBoxComponent*> SideCollisions;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		TMap<UStaticMeshComponent*, float> SideTooltipsWithAlpha;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		TArray<EMoveablePillarDirections> DisabledDirections;


public:
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Outlineable Interface" )
		void Outline( bool DisplayValue );
	
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Useable Interface" )
		void Use();

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Useable Interface" )
		void StopUsing();

private: 
	bool bIsActive = false;
	bool bIsMovingPlayer = false;
	bool bHasResetOutlineSides = true;

	FTransform PlayerGotoTransform;

	AKioCharacter* KioCharacter;
	AKioPlayerController* KioPlayerController;

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void StartInterpolateKioTowardsArrow(UArrowComponent* ArrowToInterpolateTowards );
	void InterpolateKioTowardsArrow( float DeltaTime );
	void EndInterpolateKioTowardsArrow();

	void ResetOutlineSides( float DeltaTime );
	void OutlineSideSelector( float DeltaTime );


	UArrowComponent* FindClosestArrowToPlayer();
};
