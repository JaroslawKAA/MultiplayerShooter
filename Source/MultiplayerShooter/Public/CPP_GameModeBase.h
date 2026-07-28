// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPP_GameModeBase.generated.h"

class ACPP_LobbyPlayerController;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSHOOTER_API ACPP_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<APlayerController*> PlayerControllers;
	
public:
	UFUNCTION(BlueprintCallable)
	TArray<APlayerController*> GetAllConnectedPlayerControllers();
	
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
private:
	void SetupNewPlayer(APlayerController* NewPlayer);
};
