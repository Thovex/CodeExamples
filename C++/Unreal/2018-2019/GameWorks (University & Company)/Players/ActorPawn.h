// Copyright 2018 StrateGis Groep B.V. All Rights Reserved. 
// Class AActorPawn created by Jesse van Vliet

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Framework/World/CenterActor.h"
#include "ActorPawn.generated.h"

class UFrameworkInstance;
class AActorPlayerController;
class AFrameworkGameState;

UCLASS()
class STRATEGISGAMEWORK_API AActorPawn : public APawn
{
	GENERATED_BODY()

public:
	AActorPawn( const FObjectInitializer& ObjectInitializer );

	UFUNCTION( BlueprintCallable )
		void SetLerpPositionRotation( FVector NewLerpPosition, FRotator NewLerpRotation );

	void SetLerpPositionRotation( FVector NewLerpPosition, FQuat NewLerpRotation );

public:

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		USceneComponent* TransformSceneComponent;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		UCameraComponent* CameraComponent;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		USpringArmComponent* SpringArm;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		FVector LerpPosition;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		FRotator LerpRotation;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		FTransform InitialTransform;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
		ACenterActor* CenterActor;

	UPROPERTY( Replicated, EditAnywhere, BlueprintReadWrite )
		float ActorCapital;

protected:
	virtual void BeginPlay() override;
	void GetRequiredReferences();

private:
	AActorPlayerController * PlayerActorController;
	UFrameworkInstance* FrameworkInstance;
	AFrameworkGameState* FrameworkGameState;

public:
	virtual void Tick( float DeltaTime ) override;
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;

	UFUNCTION( Server, Reliable, WithValidation, BlueprintCallable )
		void ServerInitializePawn( AActorPlayerController* PlayerAController );

	FORCEINLINE bool ServerInitializePawn_Validate( AActorPlayerController* PlayerAController ) { return true; };

	UFUNCTION( Server, Reliable, WithValidation, BlueprintCallable )
		void ServerSetCapitalAdd( float CapitalToAdd );

	UFUNCTION( Server, Reliable, WithValidation, BlueprintCallable )
		void ServerSetCapitalSub( float CapitalToSub );

	FORCEINLINE bool ServerSetCapitalAdd_Validate( float CapitalToAdd ) { return true; };
	FORCEINLINE bool ServerSetCapitalSub_Validate( float CapitalToSub ) { return true; };


	void RotateCamera( float RotationSpeed );
	void ZoomCamera( float ZoomSpeed );

	void SetCenterInitial();

	UFUNCTION( BlueprintCallable )
		void ResetCamera();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<bool> MoveKeysPressed = { false, false, false, false };


private:

	void CommandRMB();

	// Array of 4 elements: Up, Left, Down, Right (WASD)

	FORCEINLINE void MoveUpPressed() { MoveKeysPressed[0] = true; };
	FORCEINLINE void MoveLeftPressed() { MoveKeysPressed[1] = true; };
	FORCEINLINE void MoveDownPressed() { MoveKeysPressed[2] = true; };
	FORCEINLINE void MoveRightPressed() { MoveKeysPressed[3] = true; };

	FORCEINLINE void MoveUpReleased() { MoveKeysPressed[0] = false; };
	FORCEINLINE void MoveLeftReleased() { MoveKeysPressed[1] = false; };
	FORCEINLINE void MoveDownReleased() { MoveKeysPressed[2] = false; };
	FORCEINLINE void MoveRightReleased() { MoveKeysPressed[3] = false; };

	void EndTurn();

	void ScrollUp();
	void ScrollDown();

	FORCEINLINE void SpeedActionPressed() { bHoldingShift = true; };
	FORCEINLINE void SpeedActionReleased() { bHoldingShift = false; };

	void HandleInput();
	void HandleCamera();

	bool bHoldingShift;

};
