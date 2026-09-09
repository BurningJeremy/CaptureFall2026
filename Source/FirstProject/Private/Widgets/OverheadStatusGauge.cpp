// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OverheadStatusGauge.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/CAttributeSet.h"
#include "Widgets/ValueGauge.h"

void UOverheadStatusGauge::ConfigureWithAbilitySystemComponent(class UAbilitySystemComponent* AbilitySystemComponent)
{
	if (AbilitySystemComponent)
	{
		HealthBar->SetAndBindWithAbilitySystemComponent(AbilitySystemComponent,UCAttributeSet::GetHealthAttribute(), UCAttributeSet::GetMaxHealthAttribute());
		ManaBar->SetAndBindWithAbilitySystemComponent(AbilitySystemComponent, UCAttributeSet::GetManaAttribute(), UCAttributeSet::GetMaxManaAttribute());
	}
}
