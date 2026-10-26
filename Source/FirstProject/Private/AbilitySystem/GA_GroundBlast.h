// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/CGameplayAbility.h"
#include "GameplayTagContainer.h"
#include "GA_GroundBlast.generated.h"

/**
 * 
 */
UCLASS()
class UGA_GroundBlast : public UCGameplayAbility
{
	GENERATED_BODY()
public: 
	UGA_GroundBlast();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayCue", meta=(Categories="GameplayCue"))
	FGameplayTagContainer BlastingGameplayCueTags;
	
	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	float TargetTraceRange = 2000.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	float TargetAreaRadius = 200.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* CastMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	TSubclassOf<class ATA_GroundPick> TargetActorClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Effect")
	TSubclassOf<class UGameplayEffect> DamageEffect;
	
	//UPROPERTY(EditDefaultsOnly, Category="Effect")
	//TSubclassOf<class UGameplayEffect> StunEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Effect")
	float StunDuration = 3.f;
	
	UFUNCTION()
	void TargetConfirmed(const FGameplayAbilityTargetDataHandle& TargetDatahandle);
	
	UFUNCTION()
	void TargetCancelled(const FGameplayAbilityTargetDataHandle& TargetDatahandle);
};
