// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ShooterMenuPlayerController.h"
#include "CPP_ArenaGameInstance.h"
#include "UI/CPP_UIManagerSubsystem.h"

void ACPP_ShooterMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		if (UCPP_ArenaGameInstance* AreneGI = Cast<UCPP_ArenaGameInstance>(GetGameInstance()))
		{
			if (UCPP_UIManagerSubsystem* UIManagerSubsystem = AreneGI->GetSubsystem<UCPP_UIManagerSubsystem>())
			UIManagerSubsystem->UI_ShowMainMenu();
		}
	}
}
