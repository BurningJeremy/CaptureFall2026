// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GC_HitVFX.generated.h"

/**
 * 
 */
UCLASS()
class UGC_HitVFX : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	virtual bool OnExecute_Implementation( AActor* MyTarget, const FGameplayCueParameters& Parameters) const;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	class UParticleSystem* VFX;
	
};
