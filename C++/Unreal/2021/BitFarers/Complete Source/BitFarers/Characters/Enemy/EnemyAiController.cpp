// Copyright Baer and Hoggo


#include "EnemyAiController.h"
#include "NavigationSystem.h"

void AEnemyAiController::FindPathForMoveRequest(const FAIMoveRequest& MoveRequest, FPathFindingQuery& Query, FNavPathSharedPtr& OutPath) const
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys)
	{
		FPathFindingResult PathResult = NavSys->FindPathSync(Query);
		if (PathResult.Result != ENavigationQueryResult::Error)
		{
			if (PathResult.IsSuccessful() && PathResult.Path.IsValid())
			{
				if (MoveRequest.IsMoveToActorRequest())
				{
					PathResult.Path->SetGoalActorObservation(*MoveRequest.GetGoalActor(), 100.0f);
				}

				PathResult.Path->EnableRecalculationOnInvalidation(true);
				OutPath = PathResult.Path;
				GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("COMPLEX PATHING"));
			}
		}
		else if (MoveRequest.IsUsingPathfinding())
		{
			// Try to find path directly to the target
			// TODO: Since this is called quite often, maybe we should navigate to closest reachable polygon instead?
			const bool bValidQ2 = BuildBackupPathfindingQuery(MoveRequest, Query);
			if (bValidQ2)
			{
				PathResult = NavSys->FindPathSync(Query);
				if (PathResult.Result != ENavigationQueryResult::Error)
				{
					if (PathResult.IsSuccessful() && PathResult.Path.IsValid())
					{
						if (MoveRequest.IsMoveToActorRequest())
						{
							PathResult.Path->SetGoalActorObservation(*MoveRequest.GetGoalActor(), 100.0f);
						}
						PathResult.Path->EnableRecalculationOnInvalidation(true);
						OutPath = PathResult.Path;
						GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("SIMPLE PATHING"));
					}
				}
			}
		}
// 		else
// 		{
// 			UE_VLOG(this, LogAIECNavigation, Error, TEXT("Trying to find path to %s resulted in Error")
// 				, MoveRequest.IsMoveToActorRequest() ? *GetNameSafe(MoveRequest.GetGoalActor()) : *MoveRequest.GetGoalLocation().ToString());
// 			UE_VLOG_SEGMENT(this, LogAIECNavigation, Error, GetPawn() ? GetPawn()->GetActorLocation() : FAISystem::InvalidLocation
// 				, MoveRequest.GetGoalLocation(), FColor::Red, TEXT("Failed move to %s"), *GetNameSafe(MoveRequest.GetGoalActor()));
// 		}
	}
}


bool AEnemyAiController::BuildBackupPathfindingQuery(const FAIMoveRequest& MoveRequest, FPathFindingQuery& OutQuery) const
{
	bool bResult = false;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	const ANavigationData* NavData = (NavSys == nullptr) ? nullptr : NavSys->GetAbstractNavData();

	if (NavData)
	{
		FVector GoalLocation = MoveRequest.GetGoalLocation();
		if (MoveRequest.IsMoveToActorRequest())
		{
			const INavAgentInterface* NavGoal = Cast<const INavAgentInterface>(MoveRequest.GetGoalActor());
			if (NavGoal)
			{
				const FVector Offset = NavGoal->GetMoveGoalOffset(this);
				GoalLocation = FQuatRotationTranslationMatrix(MoveRequest.GetGoalActor()->GetActorQuat(), NavGoal->GetNavAgentLocation()).TransformPosition(Offset);
			}
			else
			{
				GoalLocation = MoveRequest.GetGoalActor()->GetActorLocation();
			}
		}

		FSharedConstNavQueryFilter NavFilter = UNavigationQueryFilter::GetQueryFilter(*NavData, this, MoveRequest.GetNavigationFilter());
		OutQuery = FPathFindingQuery(*this, *NavData, GetNavAgentLocation(), GoalLocation, NavFilter);
		OutQuery.SetAllowPartialPaths(MoveRequest.IsUsingPartialPaths());

		if (GetPathFollowingComponent())
		{
			GetPathFollowingComponent()->OnPathfindingQuery(OutQuery);
		}

		bResult = true;
	}
// 	else
// 	{
// 		UE_VLOG(this, LogAIECNavigation, Warning, TEXT("Unable to find NavigationData instance while calling AAIController::BuildBackupPathfindingQuery"));
// 	}

	return bResult;
}
