// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPP_PlayerCharacter.h"
#include "Weapon/CPP_Weapon.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Controller.h"
#include "Math/RotationMatrix.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "Math/Vector2D.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACPP_PlayerCharacter::ACPP_PlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ACPP_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add mapping context
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (MappingContext)
			{
				Subsystem->AddMappingContext(MappingContext, 0);
			}
		}
	}

	// Find weapon in child objects
	if (!CurrentWeapon)
	{
		TArray<AActor*> AttachedActors;
		GetAttachedActors(AttachedActors);
		for (AActor* AttachedActor : AttachedActors)
		{
			if (AttachedActor != nullptr
				&& (AttachedActor->IsA(ACPP_Weapon::StaticClass())))
			{
				CurrentWeapon = Cast<ACPP_Weapon>(AttachedActor);
			}
		}
	}
}


// Called every frame
void ACPP_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACPP_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPP_PlayerCharacter::Move);
		}

		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPP_PlayerCharacter::Look);
		}

		if (IsADSAction)
		{
			EnhancedInput->BindAction(IsADSAction, ETriggerEvent::Started, this, &ACPP_PlayerCharacter::IsAdsStarted);
			EnhancedInput->BindAction(IsADSAction, ETriggerEvent::Completed, this,
			                          &ACPP_PlayerCharacter::IsAdsCompleted);
		}

		if (ShootAction)
		{
			EnhancedInput->BindAction(ShootAction, ETriggerEvent::Started, this, &ACPP_PlayerCharacter::Shoot);
		}
	}
}

void ACPP_PlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACPP_PlayerCharacter, bIsAds);
}

void ACPP_PlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		float YVelocity = MovementVector.Y * MovementSpeed;
		float XVelocity = MovementVector.X * MovementSpeed;
		AddMovementInput(ForwardDirection, YVelocity);
		AddMovementInput(RightDirection, XVelocity);
	}
}

void ACPP_PlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X * CameraLookSpeed);
		AddControllerPitchInput(LookAxisVector.Y * CameraLookSpeed);
	}
}

void ACPP_PlayerCharacter::IsAdsStarted(const FInputActionValue& Value)
{
	if (HasAuthority())
	{
		bIsAds = true;
	}
	else
	{
		bIsAds = true;
		Srv_SetsAds(bIsAds);
	}
}

void ACPP_PlayerCharacter::IsAdsCompleted(const FInputActionValue& Value)
{
	if (HasAuthority())
	{
		bIsAds = false;
	}
	else
	{
		bIsAds = false;
		Srv_SetsAds(bIsAds);
	}
}

void ACPP_PlayerCharacter::Shoot(const FInputActionValue& Value)
{
	if (bIsAds && CurrentWeapon)
	{
		CurrentWeapon->Shoot();
	}
}

void ACPP_PlayerCharacter::Srv_SetsAds_Implementation(bool bNewIsAds)
{
	bIsAds = bNewIsAds;
}
