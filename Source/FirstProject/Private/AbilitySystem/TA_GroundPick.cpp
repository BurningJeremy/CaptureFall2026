// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TA_GroundPick.h"
#include "FirstProject/FirstProject.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/OverlapResult.h"

ATA_GroundPick::ATA_GroundPick()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATA_GroundPick::ConfirmTargetingAndContinue()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	
	GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ObjectQueryParams,
		FCollisionShape::MakeSphere(TargetAreaRadius)
		);
	
	TSet<AActor*> TargetActors;
	IGenericTeamAgentInterface* OwnerAgentInterface = nullptr;
	if (OwningAbility)
	{
		OwnerAgentInterface = Cast<IGenericTeamAgentInterface>(OwningAbility->GetAvatarActorFromActorInfo());
	}
	
	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		if (OwnerAgentInterface)
		{
			ETeamAttitude::Type OtherTeamAttitude = OwnerAgentInterface->GetTeamAttitudeTowards(*OverlapResult.GetActor());
			if (!TargetTeamAttitudes.Contains(OtherTeamAttitude))
			{
				continue;
			}
		}
		
		TargetActors.Add(OverlapResult.GetActor());
	}

	FGameplayAbilityTargetDataHandle TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActorArray(TargetActors.Array(), false);
	TargetDataReadyDelegate.Broadcast(TargetData);
}

void ATA_GroundPick::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (PrimaryPC && PrimaryPC->IsLocalController())
	{
		SetActorLocation(GetTargetPoint());
	}
}

FVector ATA_GroundPick::GetTargetPoint() const
{
	
	
	if (!PrimaryPC || !PrimaryPC->IsLocalController())
		return GetActorLocation();
	
	FVector ViewLocation;
	FRotator ViewRotation;
	
	PrimaryPC->GetPlayerViewPoint(ViewLocation, ViewRotation);
	FVector TraceEnd = ViewLocation + ViewRotation.Vector() * TargetTraceRange;
	
	FHitResult TraceResult;
	if (!GetWorld()->LineTraceSingleByChannel(TraceResult, ViewLocation, TraceEnd, ECC_Target))
	{	
		GetWorld()->LineTraceSingleByChannel(TraceResult, TraceEnd, TraceEnd + FVector::DownVector * TNumericLimits<float>::Max(), ECC_Target);
	}
	
	if (!TraceResult.bBlockingHit)
	{
		return GetActorLocation();
	}
	
	if (bShouldDrawDebug)
	{
		DrawDebugSphere(GetWorld(), TraceResult.ImpactPoint, TargetAreaRadius, 32, FColor::Red);
	}
	
	return TraceResult.ImpactPoint;
}
