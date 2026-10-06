// Fill out your copyright notice in the Description page of Project Settings.

#include "GEP_MenuWidgetBase.h"
#include "GameFramework/PlayerController.h"

UGEP_MenuWidgetBase::UGEP_MenuWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsFocusable(true);
}

void UGEP_MenuWidgetBase::EnterUIMode()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC) { return; }
	
	FInputModeUIOnly Mode;
	//TakeWidget costruisce il Widget qualora non dovesse esistere
	Mode.SetWidgetToFocus(this->TakeWidget());
	PC->SetInputMode(Mode);
	PC->SetShowMouseCursor(true);
}

void UGEP_MenuWidgetBase::EnterGameMode() const
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC) { return; }

	PC->SetInputMode(FInputModeGameOnly());
	PC->SetShowMouseCursor(false);
}
