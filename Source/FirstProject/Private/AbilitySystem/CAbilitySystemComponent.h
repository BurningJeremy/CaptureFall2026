// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/CAbilityInputID.h"
#include "CAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class UCAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UCAbilitySystemComponent();
	
	void ApplyInitialEffects();
	void GiveInitialAbilities();
private:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effect")
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;
	
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effect")
	TMap<ECAbilityInputID, TSubclassOf<UGameplayAbility>> InitialAbilities;

	void HealthChanged (const struct FOnAttributeChangeData& ChangeData);
	
	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> DeathEffect;
};
