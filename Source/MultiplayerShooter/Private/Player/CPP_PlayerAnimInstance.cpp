// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPP_PlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <KismetAnimationLibrary.h>
#include "Player/CPP_PlayerCharacter.h"

void UCPP_PlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<ACPP_PlayerCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		MovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UCPP_PlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (!OwningCharacter || !MovementComponent)
	{
		return;
	}

	Speed = OwningCharacter->GetVelocity().Length(); // ignore Z in order to not include jump offset

	FRotator AnimRotation = OwningCharacter->GetBaseAimRotation();
	FRotator ZRotation = FRotator(0, AnimRotation.Yaw, 0);

	Direction = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), ZRotation);
	if (AnimRotation.Pitch > 90)
	{
		Pitch = AnimRotation.Pitch - 360;
	}
	else
		Pitch = AnimRotation.Pitch;
	bIsInAir = MovementComponent->IsFalling();
	bIsAds = OwningCharacter->bIsAds;
}
