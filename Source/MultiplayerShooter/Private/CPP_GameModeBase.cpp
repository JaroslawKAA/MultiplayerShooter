// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_GameModeBase.h"

#include "Player/CPP_PlayerController.h"

TArray<APlayerController*> ACPP_GameModeBase::GetAllConnectedPlayerControllers()
{
	return PlayerControllers;
}

void ACPP_GameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	SetupNewPlayer(NewPlayer);
}

void ACPP_GameModeBase::SetupNewPlayer(APlayerController* NewPlayer)
{
	if (!NewPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupNewPlayer: NewPlayer jest nullptr!"));
		return;
	}

	PlayerControllers.Add(NewPlayer);
}
