// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "CPP_S_PlayerProfile.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCPP_S_PlayerProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerProfile")
	FText PlayerName;
};
