// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CAnimInstance.h"
#include "GameFramework/Character.h"
#include"GameFramework/CharactermovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	OwningCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		OwningCharacterMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (OwningCharacter)
	{
		Speed = OwningCharacter->GetVelocity().Length();
		
		FRotator BodyRotation = OwningCharacter->GetActorRotation();
		FRotator BodyRotationDelta = UKismetMathLibrary::NormalizedDeltaRotator(BodyRotation,BodyPrevRotation);
		
		YawSpeed = BodyRotationDelta.Yaw / DeltaSeconds;
		SmoothedYawSpeed = UKismetMathLibrary::FInterpTo(SmoothedYawSpeed, YawSpeed, DeltaSeconds, YawSpeedSmoothLerpRate);
		BodyPrevRotation = BodyRotation;
		
		FRotator ControlRotation = OwningCharacter->GetBaseAimRotation();
		LookRotationOffset = UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, BodyRotation);
	}
	
	if (OwningCharacterMovementComponent)
	{
		bIsFalling = OwningCharacterMovementComponent->IsFalling();
	}
}
