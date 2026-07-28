// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"
#include "CPP_PlayerCharacter.generated.h"

class ACPP_Weapon;

UCLASS()
class MULTIPLAYERSHOOTER_API ACPP_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_PlayerCharacter();

	// -- PROPERTIES
public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	bool bIsAds = false;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player State")
	TObjectPtr<ACPP_Weapon> CurrentWeapon;

	// Movement config
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	double MovementSpeed = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess=true))
	double CameraLookSpeed = 1;

	// Mapping Context - set in blueprint (EditDefaultsOnly)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> MappingContext;

	// Actions to set in BP
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IsADSAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ShootAction;


	// --- METHODS
public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void IsAdsStarted(const FInputActionValue& Value);
	void IsAdsCompleted(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void Srv_SetsAds(bool bNewIsAds);
};
