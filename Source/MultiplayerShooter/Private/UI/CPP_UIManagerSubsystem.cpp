// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CPP_UIManagerSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/App.h"
#include "UI/CPP_UIWidgetDefaults.h"
#include "VerseVM/VVMSession.h"

UCPP_MainMenuUserWidget* UCPP_UIManagerSubsystem::GetMainMenuWidget()
{
	return MainMenuWidgetInstance;
}

void UCPP_UIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	ArenaGameInstance = Cast<UCPP_ArenaGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	UIWidgetDefaults = LoadObject<UCPP_UIWidgetDefaults>(
		nullptr, 
		TEXT("/Game/_MultiplayerShooter/Blueprints/UI/BP_UIWidgetDefaults.BP_UIWidgetDefaults"));
}

void UCPP_UIManagerSubsystem::UI_ShowMainMenu()
{
	APlayerController* LocalPlayerController = ArenaGameInstance->GetFirstLocalPlayerController(GetWorld());
	if (!MainMenuWidgetInstance)
	{
		UUserWidget* WidgetInstance = CreateWidget(LocalPlayerController, UIWidgetDefaults->MainMenu, TEXT("MainMenu"));
		MainMenuWidgetInstance = Cast<UCPP_MainMenuUserWidget>(WidgetInstance);
	}

	MainMenuWidgetInstance->AddToViewport();
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(nullptr);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	LocalPlayerController->SetInputMode(InputMode);
	LocalPlayerController->bShowMouseCursor = true;
	UE_LOG(LogTemp, Log, TEXT("Showing Main Menu"));

	ArenaGameInstance->CheckForSavedProfile();

	OnMainMenuShown.Broadcast();
}

void UCPP_UIManagerSubsystem::UI_ShowLobbyMenu()
{
	APlayerController* LocalPlayerController = ArenaGameInstance->GetFirstLocalPlayerController(GetWorld());
	if (!LobbyWidgetInstance)
	{
		UUserWidget* WidgetInstance = CreateWidget(LocalPlayerController, UIWidgetDefaults->LobbyMenu, TEXT("LobbyMenu"));
		LobbyWidgetInstance = Cast<UCPP_LobbyUserWidget>(WidgetInstance);
	}

	LobbyWidgetInstance->AddToViewport();
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(nullptr);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	LocalPlayerController->SetInputMode(InputMode);
	LocalPlayerController->bShowMouseCursor = true;
	UE_LOG(LogTemp, Log, TEXT("Showing Lobby Menu"));

	OnLobbyMenuShown.Broadcast();
}

void UCPP_UIManagerSubsystem::UI_SearchGame()
{
	ArenaGameInstance->SearchGame();
}
