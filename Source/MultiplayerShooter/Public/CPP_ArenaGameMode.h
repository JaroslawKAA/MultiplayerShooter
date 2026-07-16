// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Player/CPP_PlayerController.h"
#include "CPP_ArenaGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API ACPP_ArenaGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<ACPP_PlayerController*> PlayerControllers;

	UPROPERTY()
	UGameInstance* GameInstance;

	UPROPERTY()
	int Num_ExpectedPlayers;
	
protected:
	virtual void BeginPlay() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	void SetupNewPlayer(APlayerController* NewPlayer);
};
