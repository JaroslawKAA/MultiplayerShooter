// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CPP_Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/Engine.h"

// Sets default values
ACPP_Weapon::ACPP_Weapon(): MeshComponent(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACPP_Weapon::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();
	MeshComponent = FindComponentByClass<USkeletalMeshComponent>();
}

// Called every frame
void ACPP_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_Weapon::PlayFXs()
{
	if (MuzzleFlashEffect && MeshComponent)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			MuzzleFlashEffect,
			MeshComponent,
			"Muzzle",
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTargetIncludingScale,
			true
		);
	}

	if (MuzzleFlashSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, MuzzleFlashSound, GetActorLocation());
	}
}

void ACPP_Weapon::Shoot()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black, TEXT("Shoot() called!"));
	PlayFXs();
	FHitResult Hit;
	FTransform LineTraceStartTransform = MeshComponent->GetSocketTransform("Muzzle");
	FVector StartLocation = LineTraceStartTransform.GetLocation();
	FVector ShootForward = LineTraceStartTransform.GetRotation().GetForwardVector();
	FVector EndLocation = StartLocation + ShootForward * WeaponRange;
	World->LineTraceSingleByChannel(Hit,
	                                StartLocation,
	                                EndLocation,
	                                ECC_WorldStatic);
	DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, false, 0.5f);
}
