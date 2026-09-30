// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CGameplayAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FHitResult> UCGameplayAbility::GetHitResultsFromSweepLocationTargetData(
	const FGameplayAbilityTargetDataHandle& TargetDataHandle, float SphereSweepRadius, bool bDrawDebug,
	bool bIgnoreSelf) const
{
	 TArray<FHitResult> OutResults;
	TSet<AActor*> HitActors;
	
	for(const TSharedPtr<FGameplayAbilityTargetData>& TargetData : TargetDataHandle.Data)
	{
		FVector StartLoc = TargetData -> GetOrigin().GetTranslation();
		FVector EndLoc = TargetData->GetEndPoint();
		
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
		
		TArray<AActor*> ActorsToIgnore;
		if (bIgnoreSelf)
		{
			ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
		}
		
		TArray<FHitResult> Results;
		
		UKismetSystemLibrary::SphereTraceMultiForObjects(this, StartLoc, EndLoc, SphereSweepRadius, ObjectTypes, false, ActorsToIgnore, bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
			Results, false);
		
		for (const FHitResult& Result : Results)
		{
			if (HitActors.Contains(Result.GetActor()))
			{
				continue;
			}
			
			HitActors.Add(Result.GetActor());
			
			OutResults.Add(Result);
		}
	}
	
	return OutResults;
}
