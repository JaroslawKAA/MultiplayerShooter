// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "FindSessionsCallbackProxy.h"
#include "Templates/SharedPointer.h"
#include "SaveData/CPP_SG_PlayerProfile.h"
#include "SaveData/CPP_S_PlayerProfile.h"
#include "Session/FCPP_S_SessionResultWrapper.h"
#include "UI/CPP_MainMenuUserWidget.h"
#include "CPP_ArenaGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSessionsFound, const TArray<FCPP_S_SessionResultWrapper>&,
                                             Sessions, bool, bWasSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMainMenuShowed);


/**
 * @class UCPP_ArenaGameInstance
 * @brief Custom GameInstance class for the Multiplayer Shooter game. It handles UI management and player profile operations.
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UCPP_ArenaGameInstance : public UGameInstance
{
	GENERATED_BODY()

	// --- CONSTANT ---
	const FName SERVER_NAME = "SERVER_NAME";

	// --- PROPERTIES ---
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UCPP_MainMenuUserWidget* MainMenuWidgetInstance;

	// MainMenuWidgetInstance Getter
	UFUNCTION(BlueprintCallable)
	UCPP_MainMenuUserWidget* GetMainMenuWidget();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SaveData")
	FString SlotName;

	UPROPERTY(BlueprintReadOnly, Category = "SaveData")
	UCPP_SG_PlayerProfile* PlayerProfileSaveGame;

	UPROPERTY(BlueprintReadOnly, Category = "SaveData")
	FCPP_S_PlayerProfile PlayerProfile;

	UPROPERTY(BlueprintReadOnly, Category = "Debug")
	bool bIsLanConnection = true;

private:
	UPROPERTY()
	TArray<FCPP_S_SessionResultWrapper> SessionResults;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FDelegateHandle OnJoinSessionDelegateHandle;
	FDelegateHandle OnFindSessionDelegateHandle;

	// --- DElEGATES ---
public:
	UPROPERTY(BlueprintAssignable, Category = "Session")
	FOnSessionsFound OnSessionsFound;

	UPROPERTY(BlueprintAssignable, Category = "Session")
	FOnMainMenuShowed FOnMainMenuShown;

	// --- EVENTS ---
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnJoinSessionClicked(int32 SessionIndex);
	virtual void OnJoinSessionClicked_Implementation(int32 SessionIndex);

	UFUNCTION(BlueprintCallable)
	void UI_ShowMainMenu();

	// --- FUNCTIONS ---
public:
	UFUNCTION(BlueprintCallable)
	void CreateMPSession(FName SessionName);

	void JoinMPSession(int32 SessionIndex);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
	UFUNCTION(BlueprintCallable)
	void UI_SearchGame();

	UFUNCTION(BlueprintCallable)
	void ChangePlayerName(FText PlayerName);

	UFUNCTION(BlueprintCallable)
	void CheckForSavedProfile();

	UFUNCTION(BlueprintCallable)
	void SaveProfile();

	UFUNCTION(BlueprintCallable)
	bool ChangeConnectionType();

	UFUNCTION(BlueprintCallable)
	FText GetConnectionTypeText();

private:
	void LoadProfile();

	void OnFindSessionComplete(bool bArg);
};
