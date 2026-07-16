// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPP_PlayerController.generated.h"

/**
 * ACPP_PlayerController is a custom PlayerController class that extends
 * the base Unreal Engine APlayerController. This class is implemented
 * for managing player-specific controls and interactions in the
 * MULTIPLAYERSHOOTER game.
 */
UCLASS()
class MULTIPLAYERSHOOTER_API ACPP_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
