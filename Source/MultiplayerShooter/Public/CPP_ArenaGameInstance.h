// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SaveData/CPP_SG_PlayerProfile.h"
#include "SaveData/CPP_S_PlayerProfile.h"
#include "CPP_ArenaGameInstance.generated.h"


/**
 * @class UCPP_ArenaGameInstance
 * @brief Custom GameInstance class for the Multiplayer Shooter game. It handles UI management and player profile operations.
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UCPP_ArenaGameInstance : public UGameInstance
{
	GENERATED_BODY()


	// --- PROPERTIES
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* MainMenuWidgetInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SaveData")
	FString SlotName;

	UPROPERTY(BlueprintReadOnly, Category = "SaveData")
	UCPP_SG_PlayerProfile* PlayerProfileSaveGame;

	UPROPERTY(BlueprintReadOnly, Category = "SaveData")
	FCPP_S_PlayerProfile PlayerProfile;


	// --- FUNCTIONS
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UI_ShowMainMenu();

	UFUNCTION(BlueprintCallable)
	void UI_HostGame();

	UFUNCTION(BlueprintCallable)
	void UI_SearchGame();

	UFUNCTION(BlueprintCallable)
	void ChangePlayerName(FText PlayerName);

	UFUNCTION(BlueprintCallable)
	void CheckForSavedProfile();

	UFUNCTION(BlueprintCallable)
	void SaveProfile();

private:
	UFUNCTION()
	void LoadProfile();
};
