// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_ArenaGameInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Templates/SharedPointer.h"
#include "FindSessionsCallbackProxy.h"
#include "UI/CPP_UIWidgetDefaults.h"
#include "CPP_UIManagerSubsystem.generated.h"

class UCPP_LobbyUserWidget;
class UCPP_MainMenuUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMainMenuShowed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyMenuShowed);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MULTIPLAYERSHOOTER_API UCPP_UIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	// --- PROPERTIES ---
public:
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<UCPP_MainMenuUserWidget> MainMenuWidgetInstance;
	
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<UCPP_LobbyUserWidget> LobbyWidgetInstance;

private:
	UPROPERTY()
	UCPP_ArenaGameInstance* ArenaGameInstance;
	
	UPROPERTY()
	UCPP_UIWidgetDefaults* UIWidgetDefaults;
	
	// --- DELEGATES ---
public:
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnMainMenuShowed OnMainMenuShown;
	
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnLobbyMenuShowed OnLobbyMenuShown;

	// --- FUNCTIONS ---
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintCallable)
	void UI_ShowMainMenu();

	UFUNCTION(BlueprintCallable)
	void UI_ShowLobbyMenu();
	
	UFUNCTION(BlueprintCallable)
	void UI_SearchGame();
};
