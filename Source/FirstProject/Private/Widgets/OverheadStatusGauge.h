// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadStatusGauge.generated.h"

/**
 * 
 */
UCLASS()
class UOverheadStatusGauge : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ConfigureWithAbilitySystemComponent(class UAbilitySystemComponent* AbilitySystemComponent);
	
private:
	UPROPERTY(meta=(BindWidget))
	class UValueGauge* HealthBar;
	
	UPROPERTY(meta=(BindWidget))
	class UValueGauge* ManaBar;
};
