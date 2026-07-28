// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CPP_UIWidgetDefaults.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class MULTIPLAYERSHOOTER_API UCPP_UIWidgetDefaults : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MainMenu;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> LobbyMenu;
};
