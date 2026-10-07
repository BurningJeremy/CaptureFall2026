// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

struct FAIStimulus;
/**
 * 
 */
UCLASS()
class ACAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACAIController();
	
	virtual void OnPossess(APawn* NewPawn) override;
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	class UAIPerceptionComponent* AIPerceptionComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	class UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	class UBehaviorTree* BehaviorTree;
	
	UPROPERTY(EditDefaultsOnly, Category = "Behavior")
	FName BlackboardTargetKeyName = "Target";
	
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus);
	UFUNCTION()
	void TargetForgotten(AActor* ForgottenActor);
	
	UObject* GetCurrentTarget() const;
	
	void SetCurrentTarget(AActor* NewTarget);
	
	AActor* GetNextPerceivedActor() const;
};
