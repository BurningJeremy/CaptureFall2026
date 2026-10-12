// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindNextPatrolPoint.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UBTTask_FindNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		FVector PatrolCenter = BlackboardComp->GetValueAsVector(PatrolCenterKey.SelectedKeyName);
		FNavLocation PatrolLocation;
		if (UNavigationSystemV1::GetCurrent(&OwnerComp)->GetRandomReachablePointInRadius(PatrolCenter, FindRadius, PatrolLocation))
		{
			BlackboardComp->SetValueAsVector(NextPatrolPointKey.SelectedKeyName, PatrolLocation);
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
