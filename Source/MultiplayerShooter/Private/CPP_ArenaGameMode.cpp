// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ArenaGameMode.h"
#include "Engine/GameInstance.h"


void ACPP_ArenaGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = GetGameInstance();
}

void ACPP_ArenaGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	SetupNewPlayer(NewPlayer);
}

void ACPP_ArenaGameMode::SetupNewPlayer(APlayerController* NewPlayer)
{
	if (!NewPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetupNewPlayer: NewPlayer jest nullptr!"));
		return;
	}

	ACPP_PlayerController* CppPlayerController = Cast<ACPP_PlayerController>(NewPlayer);
	PlayerControllers.Add(CppPlayerController);

	// Num_ExpectedPlayers = ;
}
