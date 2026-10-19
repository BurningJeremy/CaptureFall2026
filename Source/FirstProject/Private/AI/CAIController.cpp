// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Components/StateTreeAIComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/CAbilitySystemNativeTags.h"

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
	//StateTreeComponent = CreateDefaultSubobject<UStateTreeAIComponent>("State Tree AI Component");
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
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACAIController::TargetPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddDynamic(this, &ACAIController::TargetForgotten);
	
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (BlackboardComponent && GetPawn())
	{
		FVector PatrolCenterLocation = GetPawn()->GetActorLocation();
		BlackboardComponent->SetValueAsVector(BlackboardPatrolCenterKeyName, PatrolCenterLocation);
	}
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
		ForgetActorIfDead(TargetActor);
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
	if (!GetCurrentTarget() && GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsVector(BlackboardLastSeenLocationKeyName, ForgottenActor->GetActorLocation());
	}
}

UObject* ACAIController::GetCurrentTarget() const
{
	if (const UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		return BlackboardComponent->GetValueAsObject(BlackboardTargetKeyName);
	}
	
	return CurrentTarget;
}

void ACAIController::SetCurrentTarget(AActor* NewTarget)
{
	if (NewTarget != CurrentTarget)
	{
		CurrentTarget = NewTarget;
		//StateTreeComponent->SendStateTreeEvent(TAG_PERCEPTION_TARGET_CHANGE);
	}
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

void ACAIController::ForgetActorIfDead(AActor* ActorToForget)
{
	const UAbilitySystemComponent*  ActorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ActorToForget);
	if (!ActorASC)
		return;
	if (ActorASC->HasMatchingGameplayTag(TAG_STAT_DEAD))
	{
		//alters the age to be the max float
		for (UAIPerceptionComponent::TActorPerceptionContainer::TIterator Iter = AIPerceptionComponent->GetPerceptualDataIterator(); Iter; ++Iter)
		{
			if (Iter->Key != ActorToForget)
			{
				continue;
			}
			
			for (FAIStimulus& Stimulus : Iter->Value.LastSensedStimuli)
			{
				Stimulus.SetStimulusAge(TNumericLimits<float>::Max());
			}
		}
	}
}
