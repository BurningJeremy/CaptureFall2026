// Fill out your copyright notice in the Description page of Project Settings.


#include "GC_HitVFX.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "InterchangeResult.h"

bool UGC_HitVFX::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	UE_LOG(LogTemp, Warning, TEXT("Triggering C++ gameplay Cue"))
	
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VFX, HitResult->ImpactPoint, UKismetMathLibrary::MakeRotFromX(HitResult->ImpactNormal));
	}
	return true;
}
