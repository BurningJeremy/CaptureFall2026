// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ValueGauge.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

#include "GameplayEffectTypes.h"

void UValueGauge::NativePreConstruct()
{
	Super::NativePreConstruct();
	ProgressBar->SetFillColorAndOpacity(BarColor);
}

void UValueGauge::SetAndBindWithAbilitySystemComponent(class UAbilitySystemComponent* AbilitySystemComponent, const struct FGameplayAttribute& Attribute, const struct FGameplayAttribute& MaxAttribute)
{
	if (AbilitySystemComponent)
	{
		bool bIsFound = false;
		float Value = AbilitySystemComponent->GetGameplayAttributeValue(Attribute, bIsFound); 
		if (!bIsFound)
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't find the attribute: %s"), *(Attribute.GetName()))
		}

		float MaxValue = AbilitySystemComponent->GetGameplayAttributeValue(MaxAttribute, bIsFound);
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't find the attribute: %s"), *(MaxAttribute.GetName()))
		}
		
		SetValue(Value, MaxValue);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UValueGauge::ValueChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxAttribute).AddUObject(this, &UValueGauge::MaxValueChanged);
	}
}

void UValueGauge::SetValue(float NewValue, float NewMaxValue)
{
	CachedValue = NewValue;
	CachedMaxValue = NewMaxValue;
	
	if (NewMaxValue == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("max value cannot be 0 for value gauge!!"))
		return;
	}
	
	ProgressBar->SetPercent(NewValue/NewMaxValue);
	
	static FNumberFormattingOptions FormattingOptions = FNumberFormattingOptions().SetMaximumFractionalDigits(0);
	
	ValueText->SetText(
		FText::Format(
			FTextFormat::FromString("{0}/{1}"),
			FText::AsNumber(NewValue, &FormattingOptions),
			FText::AsNumber(NewMaxValue, &FormattingOptions)
			)
			);
}

void UValueGauge::ValueChanged(const FOnAttributeChangeData& ChangedData)
{
	SetValue(ChangedData.NewValue, CachedMaxValue);
}

void UValueGauge::MaxValueChanged(const struct FOnAttributeChangeData& ChangedData)
{
	SetValue(CachedValue, ChangedData.NewValue);
}
