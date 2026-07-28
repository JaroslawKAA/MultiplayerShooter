// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CPP_LobbyUserWidget.h"

#include "CPP_ArenaGameInstance.h"
#include "CPP_MainMenuGameMode.h"
#include "CPP_MainMenuPlayerController.h"
#include "OnlineSubsystemUtils.h"
#include "TimerManager.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"

bool UCPP_LobbyUserWidget::Initialize()
{
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	LobbyGameMode = Cast<ACPP_LobbyGameMode>(GameMode);

	return Super::Initialize();
}

void UCPP_LobbyUserWidget::Btn_StartGame()
{
	auto GameInstance = Cast<UCPP_ArenaGameInstance>(GetGameInstance());
	auto MainMenuGameMode = Cast<ACPP_MainMenuGameMode>(GetWorld()->GetAuthGameMode());
	
	int ConnectedPlayers = MainMenuGameMode->GetAllConnectedPlayerControllers().Num();
	GameInstance->TravelToMap(ConnectedPlayers, "Map_01");
}

void UCPP_LobbyUserWidget::Btn_Back()
{
	if (GIsServer)
	{
		AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
		if (ACPP_MainMenuGameMode* MainMenuGameMode = Cast<ACPP_MainMenuGameMode>(GameMode))
		{
			APlayerController* LocalPlayerController = GetOwningLocalPlayer()->GetPlayerController(GetWorld());
			TArray<APlayerController*> PlayerControllers = MainMenuGameMode->GetAllConnectedPlayerControllers();
			for (APlayerController* PlayerController : PlayerControllers)
			{
				if (PlayerController != LocalPlayerController)
				{
					Cast<ACPP_MainMenuPlayerController>(PlayerController)->EndClientSession();
				}
			}
			
			GetWorld()->GetTimerManager().SetTimer(
				DelayTimerHandle,
				this,
				&UCPP_LobbyUserWidget::EndSession,
				0.2f,
				false);
		}
	}
	else
	{
		EndSession();
	}
}

void UCPP_LobbyUserWidget::EndSession()
{
	IOnlineSessionPtr OnlineSession = Online::GetSessionInterface(GetWorld());
	OnlineSession->DestroySession(NAME_GameSession);
	
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenuLevel"), true, FString("?listen"));
}
