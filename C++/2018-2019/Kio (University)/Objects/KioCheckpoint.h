// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KioCheckpoint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FCheckpointHitDelegate );

UCLASS()
class KIO_API AKioCheckpoint : public AActor {
	GENERATED_BODY()

public:
	AKioCheckpoint( const FObjectInitializer& ObjectInitializer );

	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Checkpoint" )
		uint8 CheckpointIndex = 0;

	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Checkpoint" )
		FString CheckpointName = "a new checkpoint!";

	UPROPERTY( BlueprintReadWrite, VisibleAnywhere )
		class UBoxComponent* CheckpointCollision;

	FString GetCheckpointName() const;
	UBoxComponent* GetCheckpointCollision() const;

	UPROPERTY( BlueprintAssignable, Category = "Checkpoint" )
		FCheckpointHitDelegate OnCheckpointHit;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void BeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult );

private:
	void BindDelegates();
	void RegisterCheckpoint();
};
