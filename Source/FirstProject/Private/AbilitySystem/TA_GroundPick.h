// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GenericTeamAgentInterface.h"
#include "TA_GroundPick.generated.h"

/**
 * 
 */
UCLASS()
class ATA_GroundPick : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:
	ATA_GroundPick();

	virtual void ConfirmTargetingAndContinue() override;
	void SetTargetAreaRadius(float NewTargetAreaRadius);
	void SetTargetAreaRange(float NewTargetAreaRange);
	void SetShouldDrawDebug(bool bNewShouldDrawDebug);
	void AddTargetTeamAttitude(ETeamAttitude::Type NewAttitude);
	
protected:
	virtual void Tick(float DeltaSeconds) override;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Visual")
	class UDecalComponent* DecalComponent;
	
	FVector GetTargetPoint() const;
	
	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	float TargetTraceRange = 2000.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	float TargetAreaRadius = 200.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Debug")
	bool bShouldDrawDebug = true;
	
	UPROPERTY(EditDefaultsOnly, Category="Targeting")
	TSet<TEnumAsByte<ETeamAttitude::Type>> TargetTeamAttitudes;
	
};
