// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_S_PlayerProfile.h"
#include "GameFramework/SaveGame.h"
#include "CPP_SG_PlayerProfile.generated.h"


/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API UCPP_SG_PlayerProfile : public USaveGame
{
	GENERATED_BODY()

	/// --- PROPERTIES
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCPP_S_PlayerProfile PlayerProfile;
};
