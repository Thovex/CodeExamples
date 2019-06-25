// Copyright 2018-2019 Prickly, All Rights Reserved.

#pragma once

#include "Kio.h"
#include "Player/KioCharacter.h"
#include "GameFramework/PlayerController.h"
#include "KioPlayerController.generated.h"

/**
 * 
 */

UENUM( BlueprintType )
enum class EKioInputState : uint8 {
	Kio_Default             UMETA( "Default Input State" ),
	Kio_Glassblowing        UMETA( "Glassblowing Input State" ),
	Kio_Pillar              UMETA( "Pillar Moving Input State" ),
	Kio_Disabled            UMETA( "Diabled Input"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FInputStateChanged, EKioInputState, KioInputState );

UCLASS()
class KIO_API AKioPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	AKioPlayerController();

	UPROPERTY( BlueprintAssignable )
		FInputStateChanged OnInputStateChanged;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera )
		float BaseTurnRate = 45.F;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera )
		float BaseLookUpRate = 45.F;

public:
	bool SetInputState( EKioInputState NewInputState );

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;
	virtual void Possess( APawn * InPawn ) override;

private:
	AKioCharacter* KioCharacter;
	EKioInputState KioInputState = EKioInputState::Kio_Default;

private:
	void MoveForward( float AxisValue );
	void MoveForwardDefault( float AxisValue );

	void MoveRight( float AxisValue );
	void MoveRightDefault( float AxisValue );

	void Jump();
	void StopJumping();

	void LookUpAtRate( float Rate );
	void TurnAtRate( float Rate );

	void Use();

	void AddControllerPitchInput( float AxisValue );
	void AddControllerYawInput( float AxisValue );

public:
	FORCEINLINE EKioInputState GetInputState() { return KioInputState; }
	FORCEINLINE AKioCharacter* GetKioCharacter() { return KioCharacter; }

};
