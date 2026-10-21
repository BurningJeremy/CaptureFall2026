// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA_GroundBlast.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "AbilitySystem/CAbilitySystemNativeTags.h"
#include "AbilitySystem/TA_GroundPick.h"
#include "AbilitySystemBlueprintLibrary.h"

UGA_GroundBlast::UGA_GroundBlast()
{
	ActivationOwnedTags.AddTag(TAG_STAT_AIMING);
	BlockAbilitiesWithTag.AddTag(TAG_ABILITY_BASICATTACK);
}

void UGA_GroundBlast::ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogTemp, Warning, TEXT("GroundBlast Activated"));
	if (!HasAuthorityOrPredictionKey(CurrentActorInfo, &CurrentActivationInfo))
	{
		K2_EndAbility();
		return;
	}
	UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, CastMontage);
	PlayMontageAndWait->OnBlendOut.AddDynamic(this, &UGA_GroundBlast::K2_EndAbility);
	PlayMontageAndWait->OnInterrupted.AddDynamic(this, &UGA_GroundBlast::K2_EndAbility);
	PlayMontageAndWait->OnCancelled.AddDynamic(this, &UGA_GroundBlast::K2_EndAbility);
	PlayMontageAndWait->OnCompleted.AddDynamic(this, &UGA_GroundBlast::K2_EndAbility);
	
	PlayMontageAndWait->ReadyForActivation();
	
	UAbilityTask_WaitTargetData* WaitTargetData =
		UAbilityTask_WaitTargetData::WaitTargetData(this, NAME_None, EGameplayTargetingConfirmation::UserConfirmed, TargetActorClass);
	
	WaitTargetData->ValidData.AddDynamic(this, &UGA_GroundBlast::TargetConfirmed);
	WaitTargetData->Cancelled.AddDynamic(this, &UGA_GroundBlast::TargetCancelled);
	WaitTargetData->ReadyForActivation();
	
	AGameplayAbilityTargetActor* TargetActor = nullptr;
	
	WaitTargetData->BeginSpawningActor(this, TargetActorClass, TargetActor);
	WaitTargetData->FinishSpawningActor(this, TargetActor);
}

void UGA_GroundBlast::TargetConfirmed(const FGameplayAbilityTargetDataHandle& TargetDatahandle)
{
	//UE_LOG(LogTemp, Warning, TEXT("Target Confirmed"))
	//for (AActor* TargetActor : UAbilitySystemBlueprintLibrary::GetAllActorsFromTargetData(TargetDatahandle))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Found Target: %s"), *(TargetActor->GetName()));
	//}
	if (K2_CommitAbility())
	{
		BP_ApplyGameplayEffectToTarget(TargetDatahandle, DamageEffect);
	}
	
	UGA_GroundBlast::K2_EndAbility();
}

void UGA_GroundBlast::TargetCancelled(const FGameplayAbilityTargetDataHandle& TargetDatahandle)
{
	UE_LOG(LogTemp, Warning, TEXT("Target Cancelled"))
	UGA_GroundBlast::K2_EndAbility();
}
