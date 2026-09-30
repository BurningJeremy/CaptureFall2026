// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GC_PlayMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "GC_PlayMontage.h"

bool UGC_PlayMontage::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	USkeletalMeshComponent* TargetSkeletalMesh = MyTarget->GetComponentByClass<USkeletalMeshComponent>();
	if (TargetSkeletalMesh)
	{
		UAnimInstance* TargetAnimInst = TargetSkeletalMesh->GetAnimInstance();
		if (TargetSkeletalMesh)
		{
			TargetAnimInst->Montage_Play(MontageToPlay);
		}
	}
	return true;
}

