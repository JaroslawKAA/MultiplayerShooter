// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_ArenaGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "SaveData/CPP_SG_PlayerProfile.h"

void UCPP_ArenaGameInstance::UI_ShowMainMenu_Implementation()
{
	APlayerController* LocalPlayerController = GetFirstLocalPlayerController(GetWorld());
	if (!MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance = CreateWidget(LocalPlayerController, MainMenuWidgetClass, TEXT("MainMenu"));
	}

	MainMenuWidgetInstance->AddToViewport();
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(nullptr);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	LocalPlayerController->SetInputMode(InputMode);
	LocalPlayerController->bShowMouseCursor = true;
	UE_LOG(LogTemp, Log, TEXT("Showing Main Menu"));

	CheckForSavedProfile();
	// In BP: Set InputPlayerName input field value, no access to this variable in cpp
}

void UCPP_ArenaGameInstance::UI_HostGame()
{
	//TODO implement function
}

void UCPP_ArenaGameInstance::UI_SearchGame()
{
	//TODO implement function
}

void UCPP_ArenaGameInstance::ChangePlayerName(FText PlayerName)
{
	PlayerProfile.PlayerName = PlayerName;
	SaveProfile();
}

void UCPP_ArenaGameInstance::CheckForSavedProfile()
{
	if(UGameplayStatics::DoesSaveGameExist(SlotName, 0))
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
	if(!PlayerProfileSaveGame)
	{
		USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UCPP_SG_PlayerProfile::StaticClass());
		PlayerProfileSaveGame = Cast<UCPP_SG_PlayerProfile>(SaveGame);
	}

	if(PlayerProfileSaveGame)
	{
		PlayerProfileSaveGame->PlayerProfile = PlayerProfile;
		UGameplayStatics::SaveGameToSlot(PlayerProfileSaveGame, SlotName, 0);
	}
}

void UCPP_ArenaGameInstance::LoadProfile()
{
	USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, 0);
	TCopyQualifiersFromTo_T<USaveGame, UCPP_SG_PlayerProfile>* PlayerProfileSaveGameTmp = Cast<UCPP_SG_PlayerProfile>(SaveGame);
	this->PlayerProfile = PlayerProfileSaveGameTmp->PlayerProfile;
}
