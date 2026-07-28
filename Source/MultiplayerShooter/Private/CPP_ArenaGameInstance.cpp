// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ArenaGameInstance.h"

#include "OnlineSubsystemUtils.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/App.h"
#include "Online/CPP_OnlineUtils.h"
#include "SaveData/CPP_SG_PlayerProfile.h"
#include "VerseVM/VVMSession.h"




void UCPP_ArenaGameInstance::OnJoinSessionClicked_Implementation(int32 SessionIndex)
{
	JoinMPSession(SessionIndex);
}

void UCPP_ArenaGameInstance::SearchGame()
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 50;
	SessionSearch->bIsLanQuery = bIsLanConnection;

	OnFindSessionDelegateHandle = SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(
		this, &UCPP_ArenaGameInstance::OnFindSessionComplete);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UCPP_ArenaGameInstance::ChangePlayerName(FText PlayerName)
{
	PlayerProfile.PlayerName = PlayerName;
	SaveProfile();
}

void UCPP_ArenaGameInstance::CheckForSavedProfile()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		LoadProfile();
	}
	else
	{
		SaveProfile();
	}
}

void UCPP_ArenaGameInstance::SaveProfile()
{
	if (!PlayerProfileSaveGame)
	{
		USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UCPP_SG_PlayerProfile::StaticClass());
		PlayerProfileSaveGame = Cast<UCPP_SG_PlayerProfile>(SaveGame);
	}

	if (PlayerProfileSaveGame)
	{
		PlayerProfileSaveGame->PlayerProfile = PlayerProfile;
		UGameplayStatics::SaveGameToSlot(PlayerProfileSaveGame, SlotName, 0);
	}
}

bool UCPP_ArenaGameInstance::ChangeConnectionType()
{
	if (bIsLanConnection)
		bIsLanConnection = false;
	else
		bIsLanConnection = true;

	return bIsLanConnection;
}

FText UCPP_ArenaGameInstance::GetConnectionTypeText()
{
	if (bIsLanConnection)
		return FText::FromString("LAN");

	return FText::FromString("Online");
}

void UCPP_ArenaGameInstance::CreateMPSession(FName SessionName)
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetWorld());

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if(ExistingSession)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance.JoinMPSession: Session already exists locally, destroying before join"));
		SessionInterface->DestroySession(NAME_GameSession);
	}
	
	// Set session
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = bIsLanConnection;
	SessionSettings.bShouldAdvertise = true;
	if (bIsLanConnection)
		SessionSettings.bUsesPresence = false;
	SessionSettings.NumPublicConnections = 4;
	SessionSettings.Set(SERVER_NAME, FString(SessionName.ToString()), EOnlineDataAdvertisementType::ViaOnlineService);

	bool bSessionCreated = SessionInterface->CreateSession(0, SessionName, SessionSettings);
	if (bSessionCreated)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("LobbyLevel"), true, FString("?listen"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ArenaGameInstance.CreateMPSession: Session Creation Failed!"));
	}
}

void UCPP_ArenaGameInstance::JoinMPSession(int32 SessionIndex)
{
	IOnlineSessionPtr SessionInterface =  Online::GetSessionInterface(GetWorld());
	
	OnJoinSessionDelegateHandle = SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(
		this, &UCPP_ArenaGameInstance::OnJoinSessionCompleted);

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if(ExistingSession)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance.JoinMPSession: Session already exists locally, destroying before join"));
		SessionInterface->DestroySession(NAME_GameSession);
	}
	
	SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[SessionIndex]);
}

void UCPP_ArenaGameInstance::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSessionPtr SessionInterface =  Online::GetSessionInterface(GetWorld());
	
	SessionInterface->OnJoinSessionCompleteDelegates.Remove(OnJoinSessionDelegateHandle);

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance.JoinMPSession: Session Join Failed!"));
		return;
	}

	FString ConnectString;
	if (!SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get connect string"));
		return;
	}

	APlayerController* PC = GetFirstLocalPlayerController(GetWorld());
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get first local player"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("PC World: %s"), *GetNameSafe(PC->GetWorld()));
	UE_LOG(LogTemp, Warning, TEXT("PC Name: %s"), *PC->GetName());

	UE_LOG(LogTemp, Warning, TEXT("ClientTravel to: %s"), *ConnectString);
	PC->ClientTravel(ConnectString, TRAVEL_Absolute);
}

void UCPP_ArenaGameInstance::TravelToMap(int32 ConnectedPlayers, FString MapName)
{
	NumConnectedPlayers = ConnectedPlayers;
	
	GetWorld()->ServerTravel(MapName + TEXT("?listen"));
}

void UCPP_ArenaGameInstance::LoadProfile()
{
	USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, 0);
	TCopyQualifiersFromTo_T<USaveGame, UCPP_SG_PlayerProfile>* PlayerProfileSaveGameTmp = Cast<
		UCPP_SG_PlayerProfile>(SaveGame);
	PlayerProfile = PlayerProfileSaveGameTmp->PlayerProfile;
}

void UCPP_ArenaGameInstance::OnFindSessionComplete(bool bWasSucces)
{
	SessionResults.Empty();

	int32 Index = 0;
	for (FOnlineSessionSearchResult SessionSearchRecord : SessionSearch->SearchResults)
	{
		FCPP_S_SessionResultWrapper WrapperRecord;
		WrapperRecord.ResultIndex = Index;

		FString ServerName;
		SessionSearchRecord.Session.SessionSettings.Get(SERVER_NAME, ServerName);
		WrapperRecord.SessionName = ServerName;

		int32 MaxPublicConnections = SessionSearchRecord.Session.SessionSettings.NumPublicConnections;
		int32 OpenPublicConnections = SessionSearchRecord.Session.NumOpenPublicConnections;
		WrapperRecord.CurrentPlayers = MaxPublicConnections - OpenPublicConnections;
		WrapperRecord.MaxPlayers = MaxPublicConnections;

		WrapperRecord.PingsInMs = SessionSearchRecord.PingInMs;

		SessionResults.Add(WrapperRecord);

		Index++;
	}
	OnSessionsFound.Broadcast(SessionResults, bWasSucces);
}
