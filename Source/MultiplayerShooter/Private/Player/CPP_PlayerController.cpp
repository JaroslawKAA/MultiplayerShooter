// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPP_PlayerController.h"

void ACPP_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController())
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(nullptr);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);
		bShowMouseCursor = false;
		SetInputMode(InputMode);
	}
}
