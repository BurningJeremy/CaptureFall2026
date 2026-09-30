// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CAbilityInputID.generated.h"

UENUM(BlueprintType)
enum class ECAbilityInputID : uint8
{
	None				UMETA(DisplayName = "None"),
	BasicAttack			UMETA(DisplayName = "BasicAttack"),
	AbilityOne			UMETA(DisplayName = "Ability One"),
	AbilityTwo			UMETA(DisplayName = "Ability Two"),
	AbilityThree		UMETA(DisplayName = "Ability Three"),
	AbilityFour			UMETA(DisplayName = "Ability Four"),
	
	
	Confirm				UMETA(DisplayName = "Confirm"),
	Cancel				UMETA(DisplayName = "Cancel")
};