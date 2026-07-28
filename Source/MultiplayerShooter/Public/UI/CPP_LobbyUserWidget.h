// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_LobbyGameMode.h"
#include "Blueprint/UserWidget.h"
#include "CPP_LobbyUserWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MULTIPLAYERSHOOTER_API UCPP_LobbyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	ACPP_LobbyGameMode* LobbyGameMode;
	
	FTimerHandle DelayTimerHandle;
	
public:
	virtual bool Initialize() override;
	
protected:
	UFUNCTION(BlueprintCallable)
	void Btn_StartGame();
	
	UFUNCTION(BlueprintCallable)
	void Btn_Back();
	
private:
	void EndSession();
};
