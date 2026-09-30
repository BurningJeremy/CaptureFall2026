// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GA_Combo.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "AbilitySystem/CAbilitySystemNativeTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"

UGA_Combo::UGA_Combo()
{
	//AbilityTags.AddTag(TAG_ABILITY_BASICATTACK);
	
	SetAssetTags(FGameplayTagContainer(TAG_ABILITY_BASICATTACK));
	BlockAbilitiesWithTag.AddTag(TAG_ABILITY_BASICATTACK);
}

void UGA_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageAndWaitTask = 
					UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ComboMontage);

		PlayMontageAndWaitTask->OnCompleted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayMontageAndWaitTask->OnCancelled.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayMontageAndWaitTask->OnInterrupted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayMontageAndWaitTask->OnBlendOut.AddDynamic(this, &UGA_Combo::K2_EndAbility);

		PlayMontageAndWaitTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* WaitComboChangeEvent = 
			UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_ABILITY_COMBO_CHANGE, nullptr, false, false);

		WaitComboChangeEvent->EventReceived.AddDynamic(this, &UGA_Combo::HandleComboChange);

		WaitComboChangeEvent->ReadyForActivation();
	}
	
	SetupWaitInputPress();
	
	if (K2_HasAuthority())
	{
		UAbilityTask_WaitGameplayEvent* WaitDamageEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_ABILITY_COMBO_DAMAGE);
		WaitDamageEvent->EventReceived.AddDynamic(this, &UGA_Combo::DoDamage);
		WaitDamageEvent->ReadyForActivation();
	}
}

void UGA_Combo::HandleComboChange(FGameplayEventData EventData)
{
	UE_LOG(LogTemp, Warning, TEXT("HandleComboChange called"))
	FGameplayTag EventTag = EventData.EventTag;
	if (EventTag == TAG_ABILITY_COMBO_CHANGE_End)
	{
		NextComboName = NAME_None;
		UE_LOG(LogTemp, Warning, TEXT("Next Combo is None"))
		return;
	}
	
	TArray<FName> TagNames;
	UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, TagNames);
	NextComboName = TagNames.Last();
	
	UE_LOG(LogTemp, Warning, TEXT("Next Combo Name changed to:  %s"), *(NextComboName.ToString()));	
}

void UGA_Combo::SetupWaitInputPress()
{
	UAbilityTask_WaitInputPress* WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);
	WaitInputPress->OnPress.AddDynamic(this, &UGA_Combo::HandleComboInputPress);
	WaitInputPress->ReadyForActivation();
}

void UGA_Combo::HandleComboInputPress(float TimeWaited)
{
	SetupWaitInputPress();
	if (NextComboName == NAME_None)
	{
		return;
	}
	
	if (UAnimInstance* AnimInstance = GetCurrentActorInfo()->GetAnimInstance())
	{
		AnimInstance-> Montage_SetNextSection(AnimInstance->Montage_GetCurrentSection(ComboMontage), NextComboName, ComboMontage);
	}
}

void UGA_Combo::DoDamage(FGameplayEventData EventData)
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to do Damage"))
	TArray<FHitResult> HitResults = GetHitResultsFromSweepLocationTargetData(EventData.TargetData, 30.f, true);
	for (const FHitResult& HitResult : HitResults)
	{
		TSubclassOf<UGameplayEffect> DamageEffect = GetDamageEffectForCurrentCombo();
		
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffect, GetAbilityLevel(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo()));
		
		FGameplayEffectContextHandle EffectContextHandle = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
		EffectContextHandle.AddHitResult(HitResult);
		EffectSpecHandle.Data->SetContext(EffectContextHandle);
		
		ApplyGameplayEffectSpecToTarget(
			GetCurrentAbilitySpecHandle(),
			GetCurrentActorInfo(),
			GetCurrentActivationInfo(),
			EffectSpecHandle,
			UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(HitResult.GetActor())
			);
	}
}

TSubclassOf<class UGameplayEffect> UGA_Combo::GetDamageEffectForCurrentCombo() const
{
	if (UAnimInstance* OwnerAnimInstance = GetCurrentActorInfo()->GetAnimInstance())
	{
		FName CurrentComboName = OwnerAnimInstance->Montage_GetCurrentSection(ComboMontage);
		const TSubclassOf<UGameplayEffect>* FoundEffect = DamageEffects.Find(CurrentComboName);
		if (FoundEffect)
		{
			return *FoundEffect;
		}
	}
	
	return DefaultDamageEffects;
}
