// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ValueGauge.generated.h"

/**
 * 
 */
UCLASS()
class UValueGauge : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	void SetValue(float NewValue, float NewMaxValue);

private:
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* ProgressBar;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ValueText;
	
	UPROPERTY(EditAnywhere, Category = "ValueGauge")
	FLinearColor BarColor;
};
