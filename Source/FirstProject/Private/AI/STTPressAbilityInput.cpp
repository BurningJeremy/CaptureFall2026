// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STTPressAbilityInput.h"
#include "AbilitySystemComponent.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FSTTPressAbilityInput::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't find the ability system component!"))
		return EStateTreeRunStatus::Failed;
	}
	
	InstanceData.AbilitySystemComponent->PressInputID((int32)InstanceData.InputToPress);
	return EStateTreeRunStatus::Succeeded;
}
