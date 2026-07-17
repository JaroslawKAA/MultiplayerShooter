// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "FCPP_S_SessionResultWrapper.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FCPP_S_SessionResultWrapper
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 ResultIndex = -1;
	
	UPROPERTY(BlueprintReadWrite)
	FString SessionName;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentPlayers;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxPlayers;

	UPROPERTY(BlueprintReadWrite)
	int32 PingsInMs;
	
};
