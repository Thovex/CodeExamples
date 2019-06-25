// Copyright 2018-2019 Prickly, All Rights Reserved.

#pragma once

#include "Kio.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "KioCharacter.generated.h"

class UNearKioFinderComponent;

UCLASS( config = Game )
class AKioCharacter : public ACharacter {
	GENERATED_BODY()

public:

	AKioCharacter();

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
		class USpringArmComponent* CameraBoom;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
		class UCameraComponent* FollowCamera;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = ( AllowPrivateAccess = "true" ) )
		class USphereComponent* NearKioFinderCollision;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = ( AllowPrivateAccess = "true" ) )
		class UNearKioFinderComponent* NearKioFinderComponent;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly) 
		AActor* CurrentSelectedActor = NULL;

public:
	void SetCurrentSelectedActor( AActor* ActorToSelect );

	void UseSelectedActor();
	void StopUsingSelectedActor();

protected:
	virtual void BeginPlay() override;

private:

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE class UNearKioFinderComponent* GetNearFinderComponent() const { return NearKioFinderComponent; }

};

