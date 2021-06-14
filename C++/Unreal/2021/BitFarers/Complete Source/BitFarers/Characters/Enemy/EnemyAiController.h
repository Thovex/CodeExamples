// Copyright Baer and Hoggo

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAiController.generated.h"

/**
 * 
 */
UCLASS()
class BITFARERS_API AEnemyAiController : public AAIController
{
	GENERATED_BODY()

	virtual void FindPathForMoveRequest(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query, FNavPathSharedPtr& OutPath) const override;
	bool BuildBackupPathfindingQuery(const FAIMoveRequest& MoveRequest, FPathFindingQuery& OutQuery) const;
};
