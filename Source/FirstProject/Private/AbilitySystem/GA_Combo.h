// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/CGameplayAbility.h"
#include "GA_Combo.generated.h"

/**
 * 
 */
UCLASS()
class UGA_Combo : public UCGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Combo();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData) override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* ComboMontage;
	
	UFUNCTION()
	void HandleComboChange(FGameplayEventData EventData);
	
	FName NextComboName;
	
	void SetupWaitInputPress();
	
	UFUNCTION()
	void HandleComboInputPress(float TimeWaited);
	
	UFUNCTION()
	void DoDamage(FGameplayEventData EventData);
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FName, TSubclassOf<class UGameplayEffect>> DamageEffects;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<class UGameplayEffect> DefaultDamageEffects;
	
	TSubclassOf<class UGameplayEffect> GetDamageEffectForCurrentCombo() const;
};
