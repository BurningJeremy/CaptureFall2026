// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FORCEINLINE bool IsMoving() const { return Speed > 0;}
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FORCEINLINE bool IsNotMoving() const { return Speed == 0;}
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FORCEINLINE float GetLookYawOffset() const { return LookRotationOffset.Yaw;}
	
	UFUNCTION(BlueprintCallable, meta =(BlueprintThreadSafe))
	FORCEINLINE float GetLookPitchOffset() const { return LookRotationOffset.Pitch; }
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FORCEINLINE bool IsOnGround() const { return !bIsFalling; }
	
private:
	UPROPERTY()
	class ACharacter* OwningCharacter;
		
	UPROPERTY()
	class UCharacterMovementComponent* OwningCharacterMovementComponent;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	float Speed;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	bool bIsFalling;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	float YawSpeed;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	float SmoothedYawSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animaiton")
	float YawSpeedSmoothLerpRate = 2.f;
	
	FRotator BodyPrevRotation;
	
	UPROPERTY()
	FRotator LookRotationOffset;
};
