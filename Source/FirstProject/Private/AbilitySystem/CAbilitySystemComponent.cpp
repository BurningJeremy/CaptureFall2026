// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CAbilitySystemComponent.h"

void UCAbilitySystemComponent::ApplyInitialEffects()
{
	if (!GetOwner() || !GetOwner()-> HasAuthority())
		return;
	
	for (const TSubclassOf<UGameplayEffect>& InitialEffectClass : InitialEffects)
	{
		FGameplayEffectSpecHandle EffectSpec = MakeOutgoingSpec(InitialEffectClass, 1, MakeEffectContext());
		ApplyGameplayEffectSpecToSelf(*EffectSpec.Data);
	}
}
