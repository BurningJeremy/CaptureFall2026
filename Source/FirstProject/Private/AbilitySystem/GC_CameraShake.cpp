// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GC_CameraShake.h"

bool UGC_CameraShake::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	APlayerController* TargetPlayerController = Cast <APlayerController>(MyTarget->GetOwner());
	if (TargetPlayerController)
	{
		TargetPlayerController->ClientStartCameraShake(CameraShake);
	}
	return true;
}
