// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ArenaGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/App.h"
#include "SaveData/CPP_SG_PlayerProfile.h"
#include "VerseVM/VVMSession.h"


void UCPP_ArenaGameInstance::UI_ShowMainMenu_Implementation()
{
	APlayerController* LocalPlayerController = GetFirstLocalPlayerController(GetWorld());
	if (!MainMenuWidgetInstance)
	{
		UUserWidget* WidgetInstance = CreateWidget(LocalPlayerController, MainMenuWidgetClass, TEXT("MainMenu"));
		MainMenuWidgetInstance = Cast<UCPP_MainMenuUserWidget>(WidgetInstance);
	}

	MainMenuWidgetInstance->AddToViewport();
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(nullptr);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	LocalPlayerController->SetInputMode(InputMode);
	LocalPlayerController->bShowMouseCursor = true;
	UE_LOG(LogTemp, Log, TEXT("Showing Main Menu"));

	CheckForSavedProfile();
	// In BP: Set InputPlayerName input field value,
	// no access to this variable in cpp because WidgetIs implemented in BP
}

UCPP_MainMenuUserWidget* UCPP_ArenaGameInstance::GetMainMenuWidget()
{
	return MainMenuWidgetInstance;
}

void UCPP_ArenaGameInstance::OnJoinSessionClicked_Implementation(int32 SessionIndex)
{
	JoinMPSession(SessionIndex);
}

void UCPP_ArenaGameInstance::UI_SearchGame()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("No OnlineSubsystem found!"))
		return;
	}

	IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (!SessionInterface)
	{
		UE_LOG(LogTemp, Error, TEXT("No SessionInterface found!"))
		return;
	}

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
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("No OnlineSubsystem found!"))
		return;
	}

	IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (!SessionInterface)
	{
		UE_LOG(LogTemp, Error, TEXT("No SessionInterface found!"))
		return;
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
		UGameplayStatics::OpenLevel(GetWorld(), FName("LobbyLevel"), true, FString("listen"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance.CreateMPSession: Session Creation Failed!"));
	}
}

void UCPP_ArenaGameInstance::OnJoinSessionCompleated(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem) return;

	IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (!SessionInterface.IsValid()) return;

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

	APlayerController* PC = GetFirstLocalPlayerController();
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get first local player"));
		return;
	}

	PC->ClientTravel(ConnectString, TRAVEL_Absolute);
}

void UCPP_ArenaGameInstance::JoinMPSession(int32 SessionIndex)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("No OnlineSubsystem found!"))
		return;
	}

	IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (!SessionInterface)
	{
		UE_LOG(LogTemp, Error, TEXT("No SessionInterface found!"))
		return;
	}
	OnJoinSessionDelegateHandle = SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(
		this, &UCPP_ArenaGameInstance::OnJoinSessionCompleated);

	SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[SessionIndex]);
}

void UCPP_ArenaGameInstance::LoadProfile()
{
	USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, 0);
	TCopyQualifiersFromTo_T<USaveGame, UCPP_SG_PlayerProfile>* PlayerProfileSaveGameTmp = Cast<
		UCPP_SG_PlayerProfile>(SaveGame);
	this->PlayerProfile = PlayerProfileSaveGameTmp->PlayerProfile;
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
