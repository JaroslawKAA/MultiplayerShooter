// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_MainMenuPlayerController.h"
#include "CPP_ArenaGameInstance.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CPP_UIManagerSubsystem.h"

void ACPP_MainMenuPlayerController::BeginPlay()
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

void ACPP_MainMenuPlayerController::EndClientSession()
{
	IOnlineSessionPtr OnlineSession = Online::GetSessionInterface(GetWorld());
	OnlineSession->DestroySession(NAME_GameSession);
	
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenuLevel"), true, FString("?listen"));
}
