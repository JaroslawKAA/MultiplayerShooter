// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_GameModeBase.h"
#include "GameFramework/GameMode.h"
#include "CPP_MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API ACPP_MainMenuGameMode : public ACPP_GameModeBase
{
	GENERATED_BODY()

public:
	bool bAllPlayersConnected = false;

protected:
	virtual void BeginPlay() override;
	
};
