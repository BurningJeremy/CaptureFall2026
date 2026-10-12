// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_PressAbilityInput.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

EBTNodeResult::Type UBTTask_PressAbilityInput::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if (AIOwner)
	{
		UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AIOwner->GetPawn());
		if (OwnerASC)
		{
			OwnerASC->PressInputID((int32)InputToPress);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
	
	
}
