// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "AbilitySystem/CAbilityInputID.h"
#include "STTPressAbilityInput.generated.h"

USTRUCT()
struct FStateTreePressAbilityInputInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Parameter")
	class UAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere, Category = "Parameter")
	ECAbilityInputID InputToPress;
};

USTRUCT(meta=(DisplayName = "Press Ability Input ID"))
struct FSTTPressAbilityInput : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()
	using FInstanceDataType = FStateTreePressAbilityInputInstanceData;
	virtual const UStruct* GetInstanceDataType() const override {return FInstanceDataType::StaticStruct();}
	
public:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	
};