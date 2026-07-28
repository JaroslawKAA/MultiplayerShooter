// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPP_LobbyPlayerController.h"
#include "CPP_ArenaGameInstance.h"
#include "UI/CPP_UIManagerSubsystem.h"

void ACPP_LobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		if (UCPP_ArenaGameInstance* AreneGI = Cast<UCPP_ArenaGameInstance>(GetGameInstance()))
		{
			if (UCPP_UIManagerSubsystem* UIManagerSubsystem = AreneGI->GetSubsystem<UCPP_UIManagerSubsystem>())
				UIManagerSubsystem->UI_ShowLobbyMenu();
		}
	}
}
