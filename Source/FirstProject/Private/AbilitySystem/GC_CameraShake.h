// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GC_CameraShake.generated.h"

/**
 * 
 */
UCLASS()
class UGC_CameraShake : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TSubclassOf<UCameraShakeBase> CameraShake;
};
