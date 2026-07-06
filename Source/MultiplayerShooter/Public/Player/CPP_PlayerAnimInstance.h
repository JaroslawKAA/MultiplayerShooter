// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPP_PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UCPP_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsAds = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Pitch;

private:
	UPROPERTY()
	TObjectPtr<class ACPP_PlayerCharacter> OwningCharacter;

	UPROPERTY()
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;
};
