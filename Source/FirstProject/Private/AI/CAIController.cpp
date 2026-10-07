// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

ACAIController::ACAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception Component");
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight Config");
	
	SightConfig-> DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1200.f;
	
	//how long before AI forget you when out of sight.
	SightConfig->SetMaxAge(5.0f);
	
	SightConfig->PeripheralVisionAngleDegrees = 180.0f;
	
	AIPerceptionComponent->ConfigureSense(*SightConfig);
}

void ACAIController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	
	SetGenericTeamId(FGenericTeamId(0));
	
	IGenericTeamAgentInterface* PawnTeamInterface = Cast<IGenericTeamAgentInterface>(NewPawn);
	if (PawnTeamInterface)
	{
		PawnTeamInterface->SetGenericTeamId(GetGenericTeamId());
	}
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACAIController::TargetPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ACAIController::TargetForgotten);
}

void ACAIController::TargetPerceptionUpdated(AActor* TargetActor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		if (!GetCurrentTarget())
		{
			SetCurrentTarget(TargetActor);
		}
	}
	else
	{
		if (GetCurrentTarget() == TargetActor)
		{
			SetCurrentTarget(nullptr);
		}
	}
}

void ACAIController::TargetForgotten(AActor* ForgottenActor) 
{
	if (!ForgottenActor)
		return;
	
	if (GetCurrentTarget() == ForgottenActor)
	{
		SetCurrentTarget(GetNextPerceivedActor());
	}
}

UObject* ACAIController::GetCurrentTarget() const
{
	if (const UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		return BlackboardComponent->GetValueAsObject(BlackboardTargetKeyName);
	}
	
	return nullptr;
}

void ACAIController::SetCurrentTarget(AActor* NewTarget)
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		if (NewTarget)
		{
			BlackboardComponent->SetValueAsObject(BlackboardTargetKeyName, NewTarget);
		}
		else
		{
			BlackboardComponent->ClearValue(BlackboardTargetKeyName);
		}
	}
}

AActor* ACAIController::GetNextPerceivedActor() const
{
	if (PerceptionComponent)
	{
		TArray<AActor*> Actors;
		AIPerceptionComponent->GetPerceivedHostileActors(Actors);
		if (Actors.Num() != 0)
		{
			return Actors[0];
		}
	}
	return nullptr;
}
