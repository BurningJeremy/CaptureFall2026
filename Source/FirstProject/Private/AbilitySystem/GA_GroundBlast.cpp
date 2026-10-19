// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA_GroundBlast.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/CAbilitySystemNativeTags.h"

UGA_GroundBlast::UGA_GroundBlast()
{
	ActivationOwnedTags.AddTag(TAG_STAT_AIMING);
	
}

void UGA_GroundBlast::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
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
}
