// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Tasks/StateTreeAITask.h"
#include "Templates/SubclassOf.h"

#include "STT_RotateToFaceTarget.generated.h"

class AActor;
class AAIController;

USTRUCT()
struct FStateTreeRotateToFaceTargetInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Context")
	AAIController* AIController;
	
	UPROPERTY(EditAnywhere, Category = "Context")
	AActor* Target;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	float PrecisionDegrees = 5.f;
	
	UPROPERTY(EditAnywhere)
	bool bShouldSuccessWhenFinished = true;
};

USTRUCT(meta = (DisplayName = "Rotate To Face Target", Category = "AI|Action"))
struct FSTT_RotateToFaceTarget : public FStateTreeAIActionTaskBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeRotateToFaceTargetInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
